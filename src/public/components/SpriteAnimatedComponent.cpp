//
// Created by adria on 08/10/2018.
//

#include <game-engine/components/SpriteAnimatedComponent.hpp>
#include "../private/graphics/GraphicsEngine.hpp"
#include "../../private/GameObject.hpp"

namespace GameEngine {
    void SpriteAnimatedComponent::init() {
        updateGraphicRef();
        graphic_->setGrid(columns_, rows_);
        setVisible(visible_);
        timeAcumulator_ = 0;
        index_[0] = 0;
        index_[1] = 0;
        graphic_->setModelTransform(gameObject()->position(), gameObject()->rotation(), gameObject()->scale());
        gameObject()->registerObserver(this);
    }

    PropertySetBase *SpriteAnimatedComponent::configureProperties() {
        auto *properties = new PropertySet<SpriteAnimatedComponent>(this);

        properties->add(new Property<SpriteAnimatedComponent, std::string>(
                "filePath",
                this,
                &SpriteAnimatedComponent::filepath,
                &SpriteAnimatedComponent::filepath,
                "",
                true));

        properties->add(new Property<SpriteAnimatedComponent, int>(
                "rows",
                this,
                &SpriteAnimatedComponent::rows,
                &SpriteAnimatedComponent::rows,
                1));

        properties->add(new Property<SpriteAnimatedComponent, int>(
                "columns",
                this,
                &SpriteAnimatedComponent::columns,
                &SpriteAnimatedComponent::columns,
                1));

        //get the time between frames. default is 1 sec.
        properties->add(new Property<SpriteAnimatedComponent, float>(
                "framesPerSecond",
                this,
                &SpriteAnimatedComponent::framesPerSecond,
                &SpriteAnimatedComponent::framesPerSecond,
                1.f));

        properties->add(new Property<SpriteAnimatedComponent, bool>(
                "visible",
                this,
                &SpriteAnimatedComponent::isVisible,
                &SpriteAnimatedComponent::setVisible,
                true));

        properties->add(new Property<SpriteAnimatedComponent, bool>(
                "playOnInit",
                this,
                &SpriteAnimatedComponent::playOnInit,
                &SpriteAnimatedComponent::playOnInit,
                true));

        return properties;
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
        Internal::GraphicsEngine::GetInstance().unregisterGraphic(graphic_);
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
            index_[0] ++;
            if(index_[0] >= columns_){
                index_[0] = 0;
                index_[1] ++;
            }
            if(index_[1] >= rows_){
                index_[0] = 0;
                index_[1] = 0;
            }
            graphic_->setCellIndex(index_[0], index_[1]);

            timeAcumulator_ = 0;
        }
    }

    void SpriteAnimatedComponent::resetAnimation() {
        index_[0] = 0;
        index_[1] = 0;
        timeAcumulator_ = 0;
        graphic_->setCellIndex(index_[0], index_[1]);
    }

    void SpriteAnimatedComponent::play() {
        if(!playing_) {
            playing_ = true;
            resetAnimation();
        }
    }

    bool SpriteAnimatedComponent::isPlaying() const {
        return playing_;
    }

    void SpriteAnimatedComponent::stop() {
        playing_ = false;
    }

    void SpriteAnimatedComponent::setFrame(int frame) {
        if(frame >= getFramesNum())
            throw std::invalid_argument("frame is out of bounds. The frame " +
            std::to_string(frame) + " is equal or greater than " + std::to_string(getFramesNum()));

        index_[0] = frame % columns_;
        index_[1] = frame /columns_;
        graphic_->setCellIndex(index_[0], index_[1]);
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
        if(graphic_)
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

    void SpriteAnimatedComponent::updateGraphicRef() {
        if(filePath_.empty()){
            if(graphic_)
                Internal::GraphicsEngine::GetInstance().unregisterGraphic(graphic_);
            graphicLoaded_.reset();
            graphic_.reset();
        } else {
            graphicLoaded_ = std::make_shared<Internal::Graphic>(filePath_);
            graphic_ = std::make_shared<Internal::GraphicHolder>(graphicLoaded_);
            graphic_->setGrid(columns_, rows_);
            resetAnimation();
            Internal::GraphicsEngine::GetInstance().registerGraphic(graphic_);
        }
    }
}
