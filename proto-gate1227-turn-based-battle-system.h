#pragma once
#pragma once
#include <list>

struct RawStat
{
	int value;

	RawStat(const int value) : value(value) {};
	virtual ~RawStat() = default;
};

struct RawPctStat
{
	float value;

	RawPctStat(const float pct_value) : value(pct_value) {};
	virtual ~RawPctStat() = default;
};

#pragma region BasicRawStats

struct Health : RawStat
{
	Health(const int value = 0) : RawStat(value) {};
};

struct Mana : RawStat
{
	Mana(const int value = 0) : RawStat(value) {};
};

struct Energy : RawStat
{
	Energy(const int value = 0) : RawStat(value) {};
};

struct DamagePhysicalMin : RawStat
{
	DamagePhysicalMin(const int value = 0) : RawStat(value) {};
};

struct DamagePhysicalMax : RawStat
{
	DamagePhysicalMax(const int value = 0) : RawStat(value) {};
};

struct DamageFireMin : RawStat
{
	DamageFireMin(const int value = 0) : RawStat(value) {};
};

struct DamageFireMax : RawStat
{
	DamageFireMax(const int value = 0) : RawStat(value) {};
};

struct DamageEtherMin : RawStat
{
	DamageEtherMin(const int value = 0) : RawStat(value) {};
};

struct DamageEtherMax : RawStat
{
	DamageEtherMax(const int value = 0) : RawStat(value) {};
};

struct DefensePhysicalFlat : RawStat
{
	DefensePhysicalFlat(const int value = 0) : RawStat(value) {};
};

struct DefenseFireFlat : RawStat
{
	DefenseFireFlat(const int value = 0) : RawStat(value) {};
};

struct DefenseEtherFlat : RawStat
{
	DefenseEtherFlat(const int value = 0) : RawStat(value) {};
};

////////////////////////////////////

struct Dodge : RawPctStat
{
	Dodge(const float value = 0) : RawPctStat(value) {};
};

struct Block : RawPctStat
{
	Block(const float value = 0) : RawPctStat(value) {};
};

#pragma endregion 





struct Vitality : Health, Mana
{
	Vitality() : Health(15), Mana(10) {};
	int mana() const { return Mana::value; }
	int health() const { return Health::value; }
};

class StatSet
{
	std::list<RawStat*> raw_stats_;
	std::list<RawPctStat*> raw_pct_stats_;





};

#pragma once
#include <array>
#include <stack>
#include <utility>
#include <vector>
#include <cstdarg>
#include <list>
#include <iostream>

class Damage
{
	int amount;
	int type;
};

class Item
{
public:
	int damage;
};


class Token
{

};

class TokenStack
{
	std::stack<Token> stack_;
public:
	std::stack<Token>& get() { return stack_; }
};

class CharacterModel;

class Ability
{
public:
	Ability(CharacterModel& src) : src_(src) {};
	Ability(const Ability& other) = default;
	Ability(Ability&& other) = default;
	Ability& operator=(const Ability& other)
	{
		if (this == &other)
			return *this;
		return *this;
	}
	Ability& operator=(Ability&& other) noexcept
	{
		if (this == &other)
			return *this;
		return *this;
	}
	virtual ~Ability() = default;

	int level_{ 0 };
	CharacterModel& src_;
	virtual void apply(CharacterModel& target) = 0;
};


class CharacterModel
{
public:
	//Basic stats
	int health_;
	int energy_;
	int base_dmg_;

	int appt_min_;
	int appt_max_;

	float pct_dodge_;
	float pct_block_;

	//Tokens
	TokenStack tokens_stun_;

	TokenStack tokens_stone_;
	TokenStack tokens_steel_;
	TokenStack tokens_blood_;
	TokenStack token_vile_;


	TokenStack tokens_fire_;
	TokenStack tokens_water_;
	TokenStack tokens_electric_;
	TokenStack tokens_ether_;


	//Character item set
	Item item_weapon_right_;
	Item item_weapon_left_;

	Item item_shoulder_right_;
	Item item_shoulder_left_;
	Item item_arm_right_;
	Item item_arm_left_;
	Item item_leg_right_;
	Item item_leg_left_;
	Item item_foot_right_;
	Item item_foot_left_;
	Item item_hand_right_;
	Item item_hand_left_;
	Item item_head_;
	Item item_torso_;
	Item item_waist_;
	Item item_back_;

	Item item_neck_;
	Item item_ear_right_;
	Item item_ear_left_;
	std::array<Item, 5> items_fingers_right_{ Item(),Item(),Item(),Item(), Item() };
	std::array<Item, 5> items_fingers_left_{ Item(),Item(),Item(),Item(), Item() };

	std::vector<Ability*> abilities_;

	template<class AnAbility>
	void add_ability()
	{
		abilities_.emplace_back(new AnAbility(*this));
	}

	static void use(Ability* ability, CharacterModel& target...)
	{
		ability->apply(target);
	}
};

struct WeaponDamage
{
	int base_damage;
	int item_weapon_damage;

	WeaponDamage(const CharacterModel& character)
	{
		base_damage = character.base_dmg_;
		item_weapon_damage = character.item_arm_left_.damage + character.item_arm_right_.damage;
	}

	int total() const
	{
		return  base_damage + item_weapon_damage;
	}
};



//SWING: swings your weapon causing it's damage to the target
//Swing requires: char.base_dmg()
//		char.
class Swing : public Ability
{
public:
	Swing(CharacterModel& src)
		: Ability(src)
	{
	}

	void apply(CharacterModel& target) override
	{
		target.health_ -= WeaponDamage(src_).total();
	};

};

struct Formation
{
	std::vector<CharacterModel*> chars_;

	Formation(CharacterModel& chars...) { (chars_.emplace_back(&chars)); };
};

struct CharacterAction
{
	friend bool operator==(const CharacterAction& lhs, const CharacterAction& rhs)
	{
		return lhs.priority == rhs.priority;
	}

	friend bool operator!=(const CharacterAction& lhs, const CharacterAction& rhs)
	{
		return !(lhs == rhs);
	}

	friend bool operator<(const CharacterAction& lhs, const CharacterAction& rhs)
	{
		return lhs.priority < rhs.priority;
	}

	friend bool operator<=(const CharacterAction& lhs, const CharacterAction& rhs)
	{
		return !(rhs < lhs);
	}

	friend bool operator>(const CharacterAction& lhs, const CharacterAction& rhs)
	{
		return rhs < lhs;
	}

	friend bool operator>=(const CharacterAction& lhs, const CharacterAction& rhs)
	{
		return !(lhs < rhs);
	}

	int priority;
	Ability* ability;
	CharacterModel* target;

	CharacterAction(int priority, Ability* ability, CharacterModel* target) : priority(priority), ability(ability), target(target) {}
};


class Player
{
	std::string name_;
	std::vector<CharacterModel> characters_;


	bool in_battle_{ false };
	bool in_overworld_{ false };

};

class Battle
{

	unsigned current_turn_;
public:
	std::vector<CharacterModel*> formation_attacker_;
	std::vector<CharacterModel*> formation_defender_;

	CharacterModel* character_selected_;

	std::vector<CharacterAction> actions_current_turn;

	void add_action(CharacterAction action)
	{
		actions_current_turn.emplace_back(action);
	}

	void sort_actions()
	{
		std::sort(actions_current_turn.begin(), actions_current_turn.end(), [](const CharacterAction& l, const CharacterAction& r) {return l > r; });
	}

	void play_actions()
	{
		for (auto& action : actions_current_turn)
		{
			std::cout << "action pririty:" << action.priority << "\n";
			CharacterModel::use(action.ability, *action.target);
		}
	}

};