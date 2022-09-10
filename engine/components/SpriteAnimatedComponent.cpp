//
// Created by adria on 08/10/2018.
//

#include <game-engine/components/SpriteAnimatedComponent.hpp>
#include <game-engine/internal/graphics/GraphicsEngine.hpp>
#include <game-engine/internal/GameObject.hpp>
#include <game-engine/internal/graphics/GraphicSprite.hpp>
#include <game-engine/Game.hpp>

namespace GameEngine {

    void SpriteAnimatedComponent::preInit()
    {
        setFilePathPropertyObserver("filePath", [this](){ updateGraphicRef(); });
        setPropertyObserver<geColor>("tint", [this](){ updateTint(); });
        setPropertyObserver<int>("rows", [this](){ updateGrid(); });
        setPropertyObserver<int>("columns", [this](){ updateGrid(); });
        setPropertyObserver<float>("framesPerSecond", [this](){ updateTimePerSecond(); });
        setPropertyObserver<bool>("visible", [this](){ updateVisible(); });
        setEnumPropertyObserver("anchor", [this](){ updateAnchor(); });

        graphicsEngine_ = gameObject()->game()->graphicsEngine();

        //TODO: implement Layers or improve how to register sprites
        updateGraphicRef();
        // the preInit ensure that the graphic is already created at this point
        updateVisible();
        updateAnchor();
        updateTint();
        updateGrid();
        updateTimePerSecond();
        timeAcumulator_ = 0;
        animationOffsetFrame_ = 0;
        animationFramesLength_ = getFramesNum();
        graphic_->setModelTransform(gameObject());
        gameObject()->registerObserver(this);
    }

    int SpriteAnimatedComponent::getWidth() const {
        return graphic_->getCellWidth();
    }

    int SpriteAnimatedComponent::getHeight() const {
        return graphic_->getCellHeight();
    }

    void SpriteAnimatedComponent::onEvent(const Subject<GameObjectEvent> &target, GameObjectEvent event) {
        if(event == GameObjectEvent::TransformChanged){
            graphic_->setModelTransform(gameObject());
        }
        else if(event == GameObjectEvent::ActiveChanged){
            updateVisible();
            resetAnimation();
        }
    }

    SpriteAnimatedComponent::~SpriteAnimatedComponent() {
        if(graphicsEngine_ != nullptr && graphic_)
            graphicsEngine_->unregisterGraphic(graphic_);
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

            int columns = getPropertyValue<int>("columns");
            graphic_->setCellIndex(animationCurrentFrame_ % columns, animationCurrentFrame_ / columns);

            timeAcumulator_ = 0;
        }
    }

    void SpriteAnimatedComponent::resetAnimation()
    {
        int columns = getPropertyValue<int>("columns");

        animationCurrentFrame_ = animationOffsetFrame_;
        timeAcumulator_ = 0;
        graphic_->setCellIndex(animationCurrentFrame_ % columns, animationCurrentFrame_ / columns);
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
        int columns = getPropertyValue<int>("columns");

        playing_ = false;
        animationCurrentFrame_ = animationOffsetFrame_;
        graphic_->setCellIndex(animationCurrentFrame_ % columns, animationCurrentFrame_ / columns);
    }

    void SpriteAnimatedComponent::setFrame(int frame) {
        if(frame >= getFramesNum())
            throw std::invalid_argument("frame is out of bounds. The frame " +
                                        std::to_string(frame) + " is equal or greater than " + std::to_string(getFramesNum()));

        int columns = getPropertyValue<int>("columns");

        animationCurrentFrame_ = frame;
        graphic_->setCellIndex(frame % columns, frame /columns);
    }

    int SpriteAnimatedComponent::getFramesNum() const {
        int columns = getPropertyValue<int>("columns");
        int rows = getPropertyValue<int>("rows");
        return columns * rows;
    }

    void SpriteAnimatedComponent::onGameObjectChange(GameEngine::geGameObject *oldGameObject, GameEngine::geGameObject *newGameObject) {
        if(oldGameObject)
            oldGameObject->unregisterObserver(this);

        if(newGameObject && graphic_){
            graphic_->setModelTransform(newGameObject);

            newGameObject->registerObserver(this);
        }
    }

    void SpriteAnimatedComponent::updateGrid()
    {
        if(graphic_)
        {
            int columns = getPropertyValue<int>("columns");
            int rows = getPropertyValue<int>("rows");
            graphic_->setGrid(columns, rows);
        }
    }

void SpriteAnimatedComponent::updateGraphicRef() {
    if(gameObject() == nullptr || gameObject()->game() == nullptr)
        return;

    const std::string &filePath = getFilePathPropertyValue("filePath");
    int columns = getPropertyValue<int>("columns");
    int rows = getPropertyValue<int>("rows");
    assert(columns > 0);
    assert(rows > 0);

    if(graphic_)
        graphicsEngine_->unregisterGraphic(graphic_);

    if(filePath.empty())
    {
        graphicLoaded_.reset();
        graphic_.reset();
    }
    else
    {
        graphicLoaded_ = graphicsEngine_->loadSprite(filePath);
        graphic_ = std::make_shared<Internal::GraphicHolder>(graphicLoaded_);
        graphic_->setGrid(columns, rows);
        resetAnimation();
        graphicsEngine_->registerGraphic(graphic_);
    }
}

void SpriteAnimatedComponent::updateVisible()
{
    if(graphic_){
        bool visible = getPropertyValue<bool>("visible");
        graphic_->setActive(gameObject() && gameObject()->active() && visible);
    }
}

void SpriteAnimatedComponent::updateTint()
{
    if(graphic_){
        geColor tint = getPropertyValue<geColor>("tint");
        graphic_->setTintColor(tint);
    }
}

void SpriteAnimatedComponent::updateAnchor() {
    if(graphic_)
    {
        std::string anchor = getEnumPropertyValue("anchor");
        GameEngine::Internal::GraphicAnchor graphicAnchor = Internal::parseStringToGraphicAnchor(anchor);
        graphic_->setAnchor(graphicAnchor);
    }
}

void SpriteAnimatedComponent::updateTimePerSecond()
{
    float frames = getPropertyValue<float>("framesPerSecond");
    timePerFrame_ = 1.f / frames;
}

}
