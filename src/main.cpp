#include <Geode/Geode.hpp>
#include <Geode/modify/MenuLayer.hpp>

using namespace geode::prelude;

class $modify(MyMenuLayer, MenuLayer) {
    void onMyCustomLevel(CCObject* sender) {
    // 1. FORMA CORRECTA PARA GEODE 5.X (Compatible con Android y Windows)
    // Buscamos el recurso directamente por nombre
    auto resource = Mod::get()->getSerializedResource("mi_nivel.txt");
    
    // Si usas una versión que no tiene el método anterior, esta es la alternativa:
    auto data = file::readString(Mod::get()->getResourcesDir() / "mi_nivel.txt");

    if (data.isErr()) {
        FLAlertLayer::create("Error", "Archivo mi_nivel.txt no encontrado", "OK")->show();
        return;
    }
    
    std::string levelString = data.unwrap();

    // 2. Crear el objeto del nivel (esto se queda igual)
    auto level = GJGameLevel::create();
    level->m_levelName = "Nivel Embebido";
    level->m_levelID = 100;
    level->m_creatorName = "le10x";
    level->m_levelString = levelString;
    
    level->m_difficulty = GJDifficulty::Insane; 
    level->m_stars = 0; 
    level->m_starsRequested = 0;

    // 3. Abrir la pantalla del nivel
    auto scene = LevelInfoLayer::scene(level, false);
    CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(0.5f, scene));
    
}

    bool init() {
        if (!MenuLayer::init()) return false;

        auto menu = this->getChildByID("side-menu");
        
        // 2. Corregido: Creamos el sprite y el botón de forma estándar
        auto sprite = CCSprite::createWithSpriteFrameName("GJ_playBtn_001.png");
        sprite->setScale(0.4f);

        auto btn = CCMenuItemSpriteExtra::create(
            sprite,
            this,
            menu_selector(MyMenuLayer::onMyCustomLevel)
        );

        if (menu) {
            menu->addChild(btn);
            menu->updateLayout();
        }

        return true;
    }
};
