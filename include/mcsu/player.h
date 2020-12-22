#ifndef MCSU_PLAYER_H_
#define MCSU_PLAYER_H_

#include <rlu/rlu.h>

#define PLAYER_ARMOR_EQUIPS 4
#define PLAYER_NAME_MAX 8

typedef enum ArmorEnum {
	ARMOR_NONE,
	ARMOR_SHIELD,
	ARMOR_GAMBASON,
	ARMOR_CHAINMAIL,
	ARMOR_FULLHELM,
	ARMOR_KETTLEHELM,
	ARMOR_MAILCOIF,
	ARMOR_COUNT,
} ArmorEnum;

typedef enum ArmorTypeEnum {
	ARMOR_TYPE_OTHER,
	ARMOR_TYPE_LIGHT,
	ARMOR_TYPE_HEAVY,
	ARMOR_TYPE_COUNT,
} ArmorTypeEnum;

typedef enum FormationEnum {
	FORMATION_SQUARE,
	FORMATION_SKINE,
	FORMATION_LINE,
	FORMATION_SCATTER,
	FORMATION_SURROUND,
	FORMATION_COUNT,
} FormationEnum;

typedef enum WeaponEnum {
	WEAPON_PIKE,
	WEAPON_SPEAR,
	WEAPON_BILL,
	WEAPON_SWORD,
	WEAPON_AXE,
	WEAPON_WARHAMMER,
	WEAPON_BOW,
	WEAPON_CROSSBOW,
	WEAPON_JAVELIN,
	WEAPON_LANCE,
	WEAPON_COUNT,
} WeaponEnum;

typedef enum WeaponTypeEnum {
	WEAPON_TYPE_MELEE,
	WEAPON_TYPE_RANGED,
	WEAPON_TYPE_COUNT,
} WeaponTypeEnum;

typedef enum PlayerArchetypeEnum {
	ARCHETYPE_INFANTRY,
	ARCHETYPE_CAVALRY,
	ARCHETYPE_SHOCK,
	ARCHETYPE_COUNT,
} PlayerArchetypeEnum;

typedef struct PlayerFlatStruct {
	uint32_t uid;
	uint32_t turn; // the uid of whose turn it is
	int8_t hp;
	uint8_t ac;
	uint8_t x;
	uint8_t y;
	uint8_t training;
	uint8_t damage;
	uint8_t speed;
	uint8_t dex;
	uint8_t initiative;
	uint8_t is_charging;
	uint8_t has_advantage;

	char name[PLAYER_NAME_MAX];
	uint8_t armor_enum[PLAYER_ARMOR_EQUIPS];
	uint8_t formation_enum;
	uint8_t weapon_enum;
	uint8_t weapon_type_enum;
	uint8_t archetype_enum;
} PlayerFlatStruct;

typedef struct PlayerStruct {
	PlayerFlatStruct data;
	so *object;
} PlayerStruct;

void player_init(PlayerStruct *out, char *name, PlayerArchetypeEnum archetype);
void player_cleanup(PlayerStruct *player);

int player_get_attack(PlayerStruct *player, PlayerStruct *defender);
int player_get_defense(PlayerStruct *player, PlayerStruct *attacker);

int player_get_formation_attack_bonus(PlayerStruct *player, PlayerStruct *defender);
int player_get_formation_defend_bonus(PlayerStruct *player, PlayerStruct *attacker);

int player_get_weapon_attack_bonus(PlayerStruct *player, PlayerStruct *defender);

void player_take_damage(PlayerStruct *player, int8_t amount);
int player_has_armor(PlayerStruct *player, ArmorEnum armor);
int player_has_armor_type(PlayerStruct *player, ArmorTypeEnum armor_type);

int player_get_initiative(PlayerStruct *player);
int player_get_speed(PlayerStruct *player);
int player_get_ac(PlayerStruct *player);

void player_set_armor(PlayerStruct *player, ArmorEnum armor);
void player_set_formation(PlayerStruct *player, FormationEnum formation);
void player_set_weapon(PlayerStruct *player, WeaponEnum weapon);

int player_cmp(const void *player0, const void *player1);

PlayerArchetypeEnum player_get_archetype_by_name(char *archetype);
ArmorEnum player_get_armor_by_name(char *armor);
WeaponEnum player_get_weapon_by_name(char *weapon);

#endif // MCSU_PLAYER_H_
