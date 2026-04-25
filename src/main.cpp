#include <Geode/Geode.hpp>
#include <Geode/modify/MenuLayer.hpp>

using namespace geode::prelude;

class $modify(MyMenuLayer, MenuLayer) {
    void onMyCustomLevel(CCObject* sender) {
        // 1. Leer el archivo desde los recursos del mod
        // Geode descomprime el .geode en memoria/cache automáticamente
        auto nivelDatos = mod::get()->getResourcesDir() / "mi_nivel.txt";
        
        std::string levelString;
        if (std::filesystem::exists(nivelDatos)) {
            levelString = file::readString(nivelDatos).unwrapOr("");
        }

        if (levelString.empty()) {
            FLAlertLayer::create("Error", "No se encontró el archivo del nivel", "OK")->show();
            return;
        }

        // 2. Crear el objeto del nivel
        auto level = GJGameLevel::create();
        
        // 3. Configurar datos estéticos (Seguros, sin estrellas)
        level->m_levelName = "Nivel Embebido";
        level->m_levelID = 100; // ID simulada
        level->m_creatorName = "TuNombre";
        level->m_levelString = levelString; // Aquí va el Gzip/Base64
        
        // Configuración de la cara de dificultad
        level->m_difficulty = GJDifficulty::Insane; 
        level->m_stars = 0;           // 0 estrellas para evitar baneos
        level->m_starsRequested = 0;
        level->m_levelDesc = "Este nivel vive dentro del mod.";

        // 4. Abrir la interfaz del nivel
        auto scene = LevelInfoLayer::scene(level, false);
        CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(0.5f, scene));
    }

    // Opcional: Añadir un botón al menú principal para probarlo
    bool init() {
        if (!MenuLayer::init()) return false;

        auto menu = this->getChildByID("side-menu");
        auto btn = CCMenuItemSpriteExtra::create(
            GNM(CCSprite::createWithSpriteFrameName("GJ_playBtn_001.png"), 0.4f),
            this,
            menu_selector(MyMenuLayer::onMyCustomLevel)
        );
        menu->addChild(btn);
        menu->updateLayout();

        return true;
    }
};
