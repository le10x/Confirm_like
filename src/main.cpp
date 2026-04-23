#include <Geode/Geode.hpp>
#include <Geode/modify/LikeItemLayer.hpp>
#include <Geode/modify/CommentCell.hpp>

using namespace geode::prelude;

// Hook para Likes y Dislikes
class $modify(MyLikeItemLayer, LikeItemLayer) {
    struct Fields {
        bool m_isConfirming = false;
    };

    void onLike(CCObject* sender) {
        if (m_fields->m_isConfirming || !Mod::get()->getSettingValue<bool>("confirm-like")) {
            m_fields->m_isConfirming = false;
            return LikeItemLayer::onLike(sender);
        }

        m_fields->m_isConfirming = true;
        geode::createQuickPopup(
            "Confirm", 
            "Are you sure you want to like?", 
            "Cancel", "Yes",
            [this, sender](auto, bool btn2) {
                if (btn2) {
                    this->onLike(sender);
                } else {
                    m_fields->m_isConfirming = false;
                }
            }
        );
    }

    void onDislike(CCObject* sender) {
        if (m_fields->m_isConfirming || !Mod::get()->getSettingValue<bool>("confirm-dislike")) {
            m_fields->m_isConfirming = false;
            return LikeItemLayer::onDislike(sender);
        }

        m_fields->m_isConfirming = true;
        geode::createQuickPopup(
            "Confirm", 
            "Are you sure you want to dislike?", 
            "Cancel", "Yes",
            [this, sender](auto, bool btn2) {
                if (btn2) {
                    this->onDislike(sender);
                } else {
                    m_fields->m_isConfirming = false;
                }
            }
        );
    }
};

// Hook para borrar comentarios instantáneamente
class $modify(MyCommentCell, CommentCell) {
    void onDelete(CCObject* sender) {
        if (Mod::get()->getSettingValue<bool>("no-confirm-delete")) {
            // Saltamos la confirmación llamando al callback final directamente
            this->FLAlert_Clicked(nullptr, true);
        } else {
            // Usamos el nombre correcto para la llamada original
            CommentCell::onDelete(sender);
        }
    }
};
