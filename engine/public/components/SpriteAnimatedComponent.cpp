//
// Created by adria on 08/10/2018.
//

#include <game-engine/components/SpriteAnimatedComponent.hpp>
#include "../private/graphics/GraphicsEngine.hpp"
#include "../../private/GameObject.hpp"
#include "../../private/graphics/GraphicSprite.hpp"
#include "../../private/Game.hpp"

namespace GameEngine {

    void SpriteAnimatedComponent::preInit()
    {
        graphicsEngine_ = dynamic_cast<Internal::Game*>(gameObject()->game())->graphicsEngine();

        //TODO: implement Layers or improve how to register sprites
        updateGraphicRef();
        // the preInit ensure that the graphic is already created at this point
        setVisible(visible_);
        anchor(anchor_);
        color(color_);
        timeAcumulator_ = 0;
        animationOffsetFrame_ = 0;
        animationFramesLength_ = getFramesNum();
        graphic_->setModelTransform(gameObject()->position(), gameObject()->rotation(), gameObject()->scale());
        gameObject()->registerObserver(this);
    }

    int SpriteAnimatedComponent::getWidth() const {
        return graphic_->getCellWidth();
    }

    int SpriteAnimatedComponent::getHeight() const {
        return graphic_->getCellHeight();
    }

    void SpriteAnimatedComponent::onEvent(const Subject<GameObjectEvent> &target, const GameObjectEvent &event, void *args) {
        if(event == GameObjectEvent::TransformChanged){
            graphic_->setModelTransform(gameObject()->position(), gameObject()->rotation(), gameObject()->scale());
        }
        else if(event == GameObjectEvent::ActiveChanged){
            graphic_->setActive(gameObject()->active() && visible_);
            resetAnimation();
        }
    }

    SpriteAnimatedComponent::~SpriteAnimatedComponent() {
        if(graphicsEngine_ != nullptr && graphic_)
            graphicsEngine_->unregisterGraphic(graphic_);
    }

    void SpriteAnimatedComponent::setVisible(const bool &visible) {
        visible_ = visible;
        if(graphic_)
            graphic_->setActive(gameObject() && gameObject()->active() && visible_);
    }

    bool SpriteAnimatedComponent::isVisible() const {
        return visible_;
    }

    void SpriteAnimatedComponent::Update(float elapsedTime) {
        if(!playing_)
            return;

        timeAcumulator_ += elapsedTime;
        if(timeAcumulator_ >= timePerFrame_){
            animationCurrentFrame_ ++;
            if(animationCurrentFrame_ >= animationOffsetFrame_ + animationFramesLength_){
                animationCurrentFrame_ = animationOffsetFrame_;
            }

            graphic_->setCellIndex(animationCurrentFrame_ % columns_, animationCurrentFrame_ / columns_);

            timeAcumulator_ = 0;
        }
    }

    void SpriteAnimatedComponent::resetAnimation()
    {
        animationCurrentFrame_ = animationOffsetFrame_;
        timeAcumulator_ = 0;
        graphic_->setCellIndex(animationCurrentFrame_ % columns_, animationCurrentFrame_ / columns_);
    }
    
    void SpriteAnimatedComponent::play()
    {
        if(!playing_)
        {
            animationOffsetFrame_ = 0;
            animationFramesLength_ = getFramesNum();
            playing_ = true;
            resetAnimation();
        }
    }

    void SpriteAnimatedComponent::playRange(int firstFrame, int length)
    {
        if(!playing_)
        {
            int framesNum = getFramesNum();
            if(firstFrame < 0 || firstFrame + length > framesNum)
            {
                std::ostringstream stringStream;
                stringStream << "Range is out of bounds. The frames range [" << firstFrame <<
                             "-" << (firstFrame + length) << "] is out of bounds [0-" << framesNum << "]";
                throw std::invalid_argument(stringStream.str());
            }

            animationOffsetFrame_ = firstFrame;
            animationFramesLength_ = length;
            playing_ = true;
            resetAnimation();
        }
    }

    bool SpriteAnimatedComponent::isPlaying() const {
        return playing_;
    }

    void SpriteAnimatedComponent::pause() {
        playing_ = false;
    }

    void SpriteAnimatedComponent::stop() {
        playing_ = false;
        animationCurrentFrame_ = animationOffsetFrame_;
        graphic_->setCellIndex(animationCurrentFrame_ % columns_, animationCurrentFrame_ / columns_);
    }

void SpriteAnimatedComponent::setFrame(int frame) {
        if(frame >= getFramesNum())
            throw std::invalid_argument("frame is out of bounds. The frame " +
            std::to_string(frame) + " is equal or greater than " + std::to_string(getFramesNum()));

        animationCurrentFrame_ = frame;
        graphic_->setCellIndex(frame % columns_, frame /columns_);
    }

    int SpriteAnimatedComponent::getFramesNum() const {
        return columns_ * rows_;
    }

    void SpriteAnimatedComponent::onGameObjectChange(GameEngine::geGameObject *oldGameObject, GameEngine::geGameObject *newGameObject) {
        if(oldGameObject)
            oldGameObject->unregisterObserver(this);

        if(newGameObject && graphic_){
            graphic_->setModelTransform(newGameObject->position(), newGameObject->rotation(), newGameObject->scale());

            newGameObject->registerObserver(this);
        }
    }

    void SpriteAnimatedComponent::filepath(const std::string &path) {
        filePath_ = path;
        updateGraphicRef();
    }

    std::string SpriteAnimatedComponent::filepath() const {
        return filePath_;
    }

    void SpriteAnimatedComponent::rows(const int &numRows) {
        rows_ = numRows;
        if(graphic_)
            graphic_->setGrid(columns_, rows_);
    }

    int SpriteAnimatedComponent::rows() const {
        return rows_;
    }

    void SpriteAnimatedComponent::columns(const int &numColumns) {
        columns_ = numColumns;
        if(graphic_)
            graphic_->setGrid(columns_, rows_);
    }

    int SpriteAnimatedComponent::columns() const {
        return columns_;
    }

    float SpriteAnimatedComponent::framesPerSecond() const{
        return 1.f / timePerFrame_;
    }
    void SpriteAnimatedComponent::framesPerSecond(const float &frames) {
        timePerFrame_ = 1.f / frames;
    }

    bool SpriteAnimatedComponent::playOnInit() const{
        return playing_;
    }

    void SpriteAnimatedComponent::playOnInit(const bool &play){
        playing_ = play;
    }

    std::string SpriteAnimatedComponent::anchor() const {
        return anchor_;
    }

    void SpriteAnimatedComponent::anchor(const std::string &anchor) {
        if(graphic_)
            graphic_->setAnchor(Internal::parseStringToGraphicAnchor(anchor));

        anchor_ = anchor;
    }

void SpriteAnimatedComponent::updateGraphicRef() {
    assert(columns_ > 0);
    assert(rows_ > 0);

    if(gameObject() == nullptr || gameObject()->game() == nullptr)
        return;

    if(graphic_)
        graphicsEngine_->unregisterGraphic(graphic_);

    if(filePath_.empty())
    {
        graphicLoaded_.reset();
        graphic_.reset();
    }
    else
    {
        graphicLoaded_ = std::make_shared<Internal::GraphicSprite>(filePath_);
        graphic_ = std::make_shared<Internal::GraphicHolder>(graphicLoaded_);
        graphic_->setGrid(columns_, rows_);
        resetAnimation();
        graphicsEngine_->registerGraphic(graphic_);
    }
}

void SpriteAnimatedComponent::color(const geColor &value)
{
    color_ = value;
    if(graphic_)
        graphic_->setTintColor(color_);
}

geColor SpriteAnimatedComponent::color() const
{
    return color_;
}

PropertySetBase *SpriteAnimatedComponent::getProperties() const
{
    auto *properties = new PropertySet<SpriteAnimatedComponent>();

    properties->add(new PropertyFilePath<SpriteAnimatedComponent>(
            "filePath",
            &SpriteAnimatedComponent::filepath,
            &SpriteAnimatedComponent::filepath,
            "",
            FileType::IMAGE,
            true));

    properties->add(new Property<SpriteAnimatedComponent, geColor>(
            "tint",
            &SpriteAnimatedComponent::color,
            &SpriteAnimatedComponent::color,
            geColor(1.f)));

    properties->add(new Property<SpriteAnimatedComponent, int>(
            "rows",
            &SpriteAnimatedComponent::rows,
            &SpriteAnimatedComponent::rows,
            1));

    properties->add(new Property<SpriteAnimatedComponent, int>(
            "columns",
            &SpriteAnimatedComponent::columns,
            &SpriteAnimatedComponent::columns,
            1));

    //get the time between frames. default is 1 sec.
    properties->add(new Property<SpriteAnimatedComponent, float>(
            "framesPerSecond",
            &SpriteAnimatedComponent::framesPerSecond,
            &SpriteAnimatedComponent::framesPerSecond,
            1.f));

    properties->add(new Property<SpriteAnimatedComponent, bool>(
            "visible",
            &SpriteAnimatedComponent::isVisible,
            &SpriteAnimatedComponent::setVisible,
            true));

    properties->add(new Property<SpriteAnimatedComponent, bool>(
            "playOnInit",
            &SpriteAnimatedComponent::playOnInit,
            &SpriteAnimatedComponent::playOnInit,
            true));

    properties->add(new PropertyEnum<SpriteAnimatedComponent>(
        "anchor",
        &SpriteAnimatedComponent::anchor,
        &SpriteAnimatedComponent::anchor,
        "TOP_LEFT",
        {
            "TOP_LEFT",
            "TOP_CENTER",
            "TOP_RIGHT",
            "MIDDLE_LEFT",
            "MIDDLE_CENTER",
            "MIDDLE_RIGHT",
            "BOTTOM_LEFT",
            "BOTTOM_CENTER",
            "BOTTOM_RIGHT"
        }));

    return properties;

}
}
