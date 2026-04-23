#include <Geode/Geode.hpp>
#include <Geode/modify/LikeItemLayer.hpp>
#include <Geode/modify/CommentCell.hpp>

using namespace geode::prelude;

// --- SECCIÓN DE LIKES ---
class $modify(MyLikeItemLayer, LikeItemLayer) {
    static inline bool s_isExecuting = false;

    void onLike(CCObject* sender) {
        if (s_isExecuting || !Mod::get()->getSettingValue<bool>("confirm-like")) {
            return LikeItemLayer::onLike(sender);
        }

        geode::createQuickPopup(
            "Confirm", 
            "Are you sure you want to like?", 
            "Cancel", "Yes",
            [this, sender](auto, bool btn2) {
                if (btn2) {
                    s_isExecuting = true;
                    this->onLike(sender);
                    s_isExecuting = false;
                }
            }
        );
    }

    void onDislike(CCObject* sender) {
        if (s_isExecuting || !Mod::get()->getSettingValue<bool>("confirm-dislike")) {
            return LikeItemLayer::onDislike(sender);
        }

        geode::createQuickPopup(
            "Confirm", 
            "Are you sure you want to dislike?", 
            "Cancel", "Yes",
            [this, sender](auto, bool btn2) {
                if (btn2) {
                    s_isExecuting = true;
                    this->onDislike(sender);
                    s_isExecuting = false;
                }
            }
        );
    }
};

// --- SECCIÓN PARA QUITAR CONFIRMACIÓN VANILLA DE COMENTARIOS ---
class $modify(MyCommentCell, CommentCell) {
    void onDeleteComment(CCObject* sender) {
        // Si la opción de "Quick Delete" está activada
        if (Mod::get()->getSettingValue<bool>("no-confirm-delete")) {
            // Llamamos directamente al callback de borrar sin pasar por el popup de GD
            this->FLAlert_Clicked(nullptr, true);
        } else {
            // Comportamiento normal del juego
            CommentCell::onDeleteComment(sender);
        }
    }
};
