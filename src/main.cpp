#include <Geode/Geode.hpp>
#include <Geode/modify/LikeItemLayer.hpp>

using namespace geode::prelude;

class $modify(MyLikeItemLayer, LikeItemLayer) {
    // Usamos una variable estática temporal para evitar bucles sin depender de Fields complejos
    static inline bool s_isExecuting = false;

    void onLike(CCObject* sender) {
        bool shouldConfirm = Mod::get()->getSettingValue<bool>("confirm-like");

        // Si ya estamos ejecutando o el usuario no quiere confirmación
        if (s_isExecuting || !shouldConfirm) {
            return LikeItemLayer::onLike(sender);
        }

        geode::createQuickPopup(
            "Confirm", 
            "Are you sure you want to like?", 
            "Cancel", "Yes",
            [this, sender](auto, bool btn2) {
                if (btn2) {
                    s_isExecuting = true;
                    LikeItemLayer::onLike(sender);
                    s_isExecuting = false;
                }
            }
        );
    }

    void onDislike(CCObject* sender) {
        bool shouldConfirm = Mod::get()->getSettingValue<bool>("confirm-dislike");

        if (s_isExecuting || !shouldConfirm) {
            return LikeItemLayer::onDislike(sender);
        }

        geode::createQuickPopup(
            "Confirm", 
            "Are you sure you want to dislike?", 
            "Cancel", "Yes",
            [this, sender](auto, bool btn2) {
                if (btn2) {
                    s_isExecuting = true;
                    LikeItemLayer::onDislike(sender);
                    s_isExecuting = false;
                }
            }
        );
    }

    // Nota: Para borrar comentarios se suele usar onDelete en otras clases, 
    // pero si LikeItemLayer gestiona alguna eliminación, se intercepta así:
    void onDelete(CCObject* sender) {
        bool shouldConfirm = Mod::get()->getSettingValue<bool>("confirm-comment-delete");

        if (s_isExecuting || !shouldConfirm) {
            return LikeItemLayer::onDelete(sender);
        }

        geode::createQuickPopup(
            "Confirm", 
            "Are you sure you want to delete this?", 
            "Cancel", "Yes",
            [this, sender](auto, bool btn2) {
                if (btn2) {
                    s_isExecuting = true;
                    LikeItemLayer::onDelete(sender);
                    s_isExecuting = false;
                }
            }
        );
    }
};
