#include "TipsLayer.h"
static TipsLayer* _tipslayer = nullptr;

TipsLayer* TipsLayer::getInstance()
{
	if (!_tipslayer)
	{
		_tipslayer = new TipsLayer();
	}
	return _tipslayer;
}

Layer* TipsLayer::getNEGemsLayer()
{
	auto size = Director::getInstance()->getVisibleSize();
	auto layer = Layer::create();
	//rt
	auto rt = RenderTexture::create(size.width, size.height, Texture2D::PixelFormat::RGBA8888);
	rt->setPosition(size / 2);
	rt->clear(0, 0, 0, 0.5f);
	rt->setName("rt");
	layer->addChild(rt);
	auto lis = EventListenerTouchOneByOne::create();
	lis->setSwallowTouches(true);
	lis->onTouchBegan = [layer,size](Touch* t, Event* e)->bool{
		auto pos = layer->convertTouchToNodeSpace(t);
		auto rc = Rect{ Vec2::ZERO, size };
		if (rc.containsPoint(pos))
		{
			return true;
		}
		return false;
	};
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(lis, layer);
	//_eventDispatcher->addEventListenerWithSceneGraphPriority(lis, layer);
	//tips box
	auto bg = Sprite::create("tips/tips_bg_big.png");
	bg->setPosition(size / 2);
	layer->addChild(bg);
	//close button
	auto bt = Button::create("tips/close_normal.png", "tips/close_selected.png", "");
	bt->addTouchEventListener([bg,layer](Ref* sender, Widget::TouchEventType type)->void{
		if (type == Widget::TouchEventType::ENDED)
			bg->runAction(Sequence::create(
			ScaleTo::create(0.3f, 0.1f),
			CallFunc::create([layer]()->void{
			layer->removeFromParent();
		}),
			nullptr));
	});
	bt->setPosition(bg->getContentSize() - bt->getContentSize() / 2);
	bg->addChild(bt);
	//tips
	auto tip = Sprite::create("tips/negems.png");
	tip->setPosition(bg->getContentSize() / 2);
	bg->addChild(tip);

	return layer;
}

Layer* TipsLayer::getNEFruitLayer()
{
	auto size = Director::getInstance()->getVisibleSize();
	auto layer = Layer::create();
	//rt
	auto rt = RenderTexture::create(size.width, size.height, Texture2D::PixelFormat::RGBA8888);
	rt->setPosition(size / 2);
	rt->clear(0, 0, 0, 0.5f);
	rt->setName("rt");
	layer->addChild(rt);
	auto lis = EventListenerTouchOneByOne::create();
	lis->setSwallowTouches(true);
	lis->onTouchBegan = [layer, size](Touch* t, Event* e)->bool{
		auto pos = layer->convertTouchToNodeSpace(t);
		auto rc = Rect{ Vec2::ZERO, size };
		if (rc.containsPoint(pos))
		{
			return true;
		}
		return false;
	};
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(lis, layer);
	//_eventDispatcher->addEventListenerWithSceneGraphPriority(lis, layer);
	//tips box
	auto bg = Sprite::create("tips/tips_bg_big.png");
	bg->setPosition(size / 2);
	layer->addChild(bg);
	//close button
	auto bt = Button::create("tips/close_normal.png", "tips/close_selected.png", "");
	bt->addTouchEventListener([bg, layer](Ref* sender, Widget::TouchEventType type)->void{
		if (type == Widget::TouchEventType::ENDED)
			bg->runAction(Sequence::create(
			ScaleTo::create(0.3f, 0.1f),
			CallFunc::create([layer]()->void{
			layer->removeFromParent();
		}),
			nullptr));
	});
	bt->setPosition(bg->getContentSize() - bt->getContentSize() / 2);
	bg->addChild(bt);
	//tips
	auto tip = Sprite::create("tips/nefruits.png");
	tip->setPosition(bg->getContentSize() / 2);
	bg->addChild(tip);

	return layer;
}