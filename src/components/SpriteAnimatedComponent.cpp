//
// Created by adria on 08/10/2018.
//

#include "SpriteAnimatedComponent.hpp"
#include "../graphics/GraphicsEngine.hpp"

void SpriteAnimatedComponent::init() {
    setVisible(visible_);
    timeAcumulator_ = 0;
    index_[0] = 0;
    index_[1] = 0;
}

std::shared_ptr<Component> SpriteAnimatedComponent::Clone() {
    std::shared_ptr<SpriteAnimatedComponent> clone = std::make_shared<SpriteAnimatedComponent>();
    clone->graphicLoaded_ = graphicLoaded_;
    clone->graphic_ = std::make_shared<GraphicHolder>(graphicLoaded_);
    clone->graphic_->setGrid(columns_, rows_);
    clone->timePerFrame_ = timePerFrame_;
    clone->columns_ = columns_;
    clone->rows_ = rows_;
    clone->visible_ = visible_;
    clone->playing_ = playing_;
    GraphicsEngine::GetInstance().registerGraphic(clone->graphic_);
    return clone;
}

void SpriteAnimatedComponent::fromFile(const YAML::Node &componentConfig) {
    if(!componentConfig["filePath"])
        throw std::logic_error("property filePath not defined");
    graphicLoaded_ = std::make_shared<Graphic>(componentConfig["filePath"].as<std::string>());
    //get the time between frames. default is 1 sec.
    timePerFrame_ = 1.f/componentConfig["framesPerSecond"].as<float>(1);
    columns_ = componentConfig["columns"].as<int>(1);
    rows_ = componentConfig["rows"].as<int>(1);
    visible_ = componentConfig["visible"].as<bool>(true);
    playing_ = componentConfig["playOnInit"].as<bool>(true);
}

void SpriteAnimatedComponent::SetParent(GameObject *parent) {
    if(parent_)
        parent_->unregisterObserver(this);
    parent_ = parent;

    if(parent_ && graphic_){
        graphic_->setModelTransform(parent_->getPosition(), parent_->getRotation(), parent_->getScale());

        parent_->registerObserver(this);
    }
}

int SpriteAnimatedComponent::getWidth() const {
    return graphic_->getCellWidth();
}

int SpriteAnimatedComponent::getHeight() const {
    return graphic_->getCellHeight();
}

void SpriteAnimatedComponent::onEvent(const Subject<GameObjectEvent> &target, const GameObjectEvent &event, void *args) {
    if(event == GameObjectEvent::TransformChanged){
        graphic_->setModelTransform(parent_->getPosition(), parent_->getRotation(), parent_->getScale());
    }
    else if(event == GameObjectEvent::ActiveChanged){
        graphic_->setActive(parent_->isActive() && visible_);
        resetAnimation();
    }
}

SpriteAnimatedComponent::~SpriteAnimatedComponent() {
    GraphicsEngine::GetInstance().unregisterGraphic(graphic_);
}

void SpriteAnimatedComponent::setVisible(bool visible) {
    visible_ = visible;
    graphic_->setActive(parent_->isActive() && visible_);
    resetAnimation();
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
