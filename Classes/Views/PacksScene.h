#ifndef _CHIPS_CHALLENGE_PACKS_SCENE_H_
#define _CHIPS_CHALLENGE_PACKS_SCENE_H_

#include <cocos2d.h>
#include <cocos-ext.h>

class ChipsChallengeGame;

class PacksScene : public cocos2d::Scene
{
private:
    ChipsChallengeGame* _game;
    cocos2d::Node* _buttonsNode;
    
public:
	static PacksScene* create(ChipsChallengeGame* game);
	PacksScene(ChipsChallengeGame* game);
    virtual ~PacksScene();

protected:
	virtual void onEnter() override;
	virtual void onExitTransitionDidStart() override;
	virtual void onExit() override;

	void _build();
	void _animateEntering();
	void _animateExiting();

    void _openPackCallback(cocos2d::Ref *sender);
};
    
#endif
