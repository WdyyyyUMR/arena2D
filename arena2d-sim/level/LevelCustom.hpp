#ifndef LEVELCUSTOM_H
#define LEVELCUSTOM_H

#include "Level.hpp"
#include "Wanderers.hpp"

#define LEVEL_CUSTOM_GOAL_SPAWN_AREA_BLOCK_SIZE 0.1 // maximum size of block when creating quad tree of goal spawn area

/* randomly generated level with static obstacles and optional dynamic obstacles */
class LevelCustom : public Level {
public:
    /* constructor
	 */
	LevelCustom(const LevelDef & levelDef, bool dynamic = false, bool human = false) : Level(levelDef), _dynamic(dynamic), _human(human), wanderers(levelDef){}

	/* destructor
	 */
	~LevelCustom(){}

    /* reset
     */
    void reset(bool robot_position_reset) override;

	/* update
	 */
	void update() override{
		wanderers.update();
	}

	/* render spawn area
	 * overriding to visualize spawn area for dynamic obstacles
	 */
	void renderGoalSpawn() override;

	/* provide the agent with additional data generated by level
	 * @param data any values pushed into this vector will be passed to the agent as additional observations
	 */
	void getAgentData(std::vector<float> & data) override{
		wanderers.getWandererData(data);
	}

	/* get level specific reward, called after every complete simulation step (not on every step iteration)
	 * use this function to implement custom reward functions that depend on additional metrics in the level
	 * @return agent reward 
	 */
	float getReward() override;

	/* check if robot had contact with a human
 	 * @return true if contact with human and false otherwise
	 */
	bool checkHumanContact(b2Fixture * other_fixture) override{
		return wanderers.checkHumanContact(other_fixture);
	}

private:
   /* if set to true, create dynamic obstacles (wanderers) in addition to static */
	bool _human;
	bool _dynamic;
	
	std::vector<float> _closestDistance; //current distances from robot to closest wanderers
	std::vector<float> _closestDistance_old; //last closest distances from robot to wanderers
	
	/* handles all wanderers for dynamic level */
	Wanderers wanderers;

	/* spawn area for dynamic obstacles */
	RectSpawn _dynamicSpawn;

    b2Body *
    generateRandomBodyVertical(const b2Vec2 &position, float min_radius, float max_radius, zRect *aabb);

    b2Body *
    generateRandomBodyHorizontal(const b2Vec2 &position, float min_radius, float max_radius, zRect *aabb);

};

#endif
