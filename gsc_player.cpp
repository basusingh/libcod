#include "gsc_player.hpp"

/* Functions that must present even if COMPILE_PLAYER == 0 */

int getSVSTime()
{

#if COD_VERSION == COD2_1_0
	int info_start = *(int *)0x0841FB04;
#elif COD_VERSION == COD2_1_2
	int info_start = *(int *)0x08422004;
#elif COD_VERSION == COD2_1_3
	int info_start = *(int *)0x08423084;
#endif

	return info_start;
}

#if COMPILE_PLAYER == 1

#define PLAYERSTATE_VELOCITY(playerid) (PLAYERSTATE(playerid) + 0x20)

void gsc_player_velocity_set(int id)
{
	float velocity[3];

	if ( ! stackGetParams("v", &velocity))
	{
		stackError("gsc_player_velocity_set() argument is undefined or has a wrong type");
		stackPushUndefined();
		return;
	}

	float *player_0_velocity_x = (float *)(PLAYERSTATE_VELOCITY(id) + 0);
	float *player_0_velocity_y = (float *)(PLAYERSTATE_VELOCITY(id) + 4);
	float *player_0_velocity_z = (float *)(PLAYERSTATE_VELOCITY(id) + 8);

	*player_0_velocity_x = velocity[0];
	*player_0_velocity_y = velocity[1];
	*player_0_velocity_z = velocity[2];

	stackPushInt(1);
}

void gsc_player_clientuserinfochanged(int id)
{
	ClientUserinfoChanged(id);
	stackPushInt(1);
}

void gsc_player_velocity_add(int id)
{
	float velocity[3];

	if ( ! stackGetParams("v", &velocity))
	{
		stackError("gsc_player_velocity_add() argument is undefined or has a wrong type");
		stackPushUndefined();
		return;
	}

	float *player_0_velocity_x = (float *)(PLAYERSTATE_VELOCITY(id) + 0);
	float *player_0_velocity_y = (float *)(PLAYERSTATE_VELOCITY(id) + 4);
	float *player_0_velocity_z = (float *)(PLAYERSTATE_VELOCITY(id) + 8);

	*player_0_velocity_x += velocity[0];
	*player_0_velocity_y += velocity[1];
	*player_0_velocity_z += velocity[2];

	stackPushInt(1);
}

void gsc_get_userinfo(int id)
{
	char* key;
	char* val;
	if ( ! stackGetParams("s", &key))
	{
		stackError("gsc_get_userinfo() argument is undefined or has a wrong type");
		stackPushUndefined();
		return;
	}

	int entity = PLAYERBASE(id);
	val = Info_ValueForKey((char*)entity+12, key);

	if(strlen(val))
		stackPushString(val);
	else
		stackPushUndefined();
}

void gsc_set_userinfo(int id)
{
	char* key;
	char* value;
	if ( ! stackGetParams("ss", &key, &value))
	{
		stackError("gsc_set_userinfo() one or more arguments is undefined or has a wrong type");
		stackPushUndefined();
		return;
	}

	int entity = PLAYERBASE(id);
	Info_SetValueForKey((char*)entity + 12, key, value);
	stackPushUndefined();
}

void gsc_player_velocity_get(int id)
{
	float *vectorVelocity = (float *)PLAYERSTATE_VELOCITY(id);
	stackPushVector(vectorVelocity);
}

void gsc_player_button_ads(int id)
{
	unsigned char *aim_address = (unsigned char *)(PLAYERSTATE(id) + 0x26CD);
	int aimButtonPressed = *aim_address & 0xF0; // just the first 4 bits tell the state
	stackPushInt(aimButtonPressed);
}

void gsc_player_button_left(int id)
{
	unsigned char *aim_address = (unsigned char *)(PLAYERSTATE(id) + 0x26FD);

	int leftButtonPressed = (*aim_address & 0x81)==0x81;
	stackPushInt(leftButtonPressed);
}

void gsc_player_button_right(int id)
{
	unsigned char *aim_address = (unsigned char *)(PLAYERSTATE(id) + 0x26FD);

	int rightButtonPressed = (*aim_address & 0x7F)==0x7F;
	stackPushInt(rightButtonPressed);
}

void gsc_player_button_forward(int id)
{
	unsigned char *aim_address = (unsigned char *)(PLAYERSTATE(id) + 0x26FC);

	int forwardButtonPressed = (*aim_address & 0x7F)==0x7F;
	stackPushInt(forwardButtonPressed);
}

void gsc_player_button_back(int id)
{
	unsigned char *aim_address = (unsigned char *)(PLAYERSTATE(id) + 0x26FC);

	int backButtonPressed = (*aim_address & 0x81)==0x81;
	stackPushInt(backButtonPressed);
}

void gsc_player_button_leanleft(int id)
{
	unsigned char *aim_address = (unsigned char *)(PLAYERSTATE(id) + 0x26E8);

	int leanleftButtonPressed = (*aim_address & 0x40)==0x40;
	stackPushInt(leanleftButtonPressed);
}

void gsc_player_button_leanright(int id)
{
	unsigned char *aim_address = (unsigned char *)(PLAYERSTATE(id) + 0x26E8);

	int leanrightButtonPressed = (*aim_address & 0x80)==0x80;
	stackPushInt(leanrightButtonPressed);
}

void gsc_player_button_reload(int id)
{
	unsigned char *aim_address = (unsigned char *)(PLAYERSTATE(id) + 0x26E8);

	int reloadButtonPressed = (*aim_address & 0x10)==0x10;
	stackPushInt(reloadButtonPressed);
}

void gsc_player_button_jump(int id)
{
	unsigned char *aim_address = (unsigned char *)(PLAYERSTATE(id) + 0x26E9);

	int jumpButtonPressed = (*aim_address & 0x04)==0x04;
	stackPushInt(jumpButtonPressed);
}

void gsc_player_stance_get(int id)
{
	unsigned char *stance_address = (unsigned char *)(G_ENTITY(id) + 8);
	int code = *stance_address & 0x0F; // just the last 4 bits tell the state

	char *stance;
	switch (code)
	{
	case  0:
	case  2:
		stance = "stand";
		break;

	case  4:
	case  6:
		stance = "duck";
		break;

	case  8:
	case 10:
		stance = "lie";
		break;

	default:
		stance = "unknown";
		break;
	}

	stackPushString(stance);
}

void gsc_player_spectatorclient_get(int id)
{
	int spectatorClient = *(unsigned char *)(PLAYERSTATE(id) + 0xCC);

	stackPushEntity(G_ENTITY(spectatorClient));
}

void gsc_player_getip(int id)
{

#if COD_VERSION == COD2_1_0
	int info_ip_offset = 0x6E5C8;
#elif COD_VERSION == COD2_1_2
	int info_ip_offset = 0x6E6D8;
#elif COD_VERSION == COD2_1_3
	int info_ip_offset = 0x6E6D8;
#endif

	char tmp[64];
	unsigned int ip_a, ip_b, ip_c, ip_d;

	int info_player = PLAYERBASE(id);
	ip_a = *(unsigned char *)(info_player + info_ip_offset + 0);
	ip_b = *(unsigned char *)(info_player + info_ip_offset + 1); // dafuq, its +1 but in IDA its +4 step :S
	ip_c = *(unsigned char *)(info_player + info_ip_offset + 2);
	ip_d = *(unsigned char *)(info_player + info_ip_offset + 3);

	snprintf(tmp, sizeof(tmp), "%d.%d.%d.%d", ip_a, ip_b, ip_c, ip_d);

	stackPushString(tmp);
}

void gsc_player_getping(int id)
{

#if COD_VERSION == COD2_1_0
	int info_ping_offset = 0x6E5A4;
#elif COD_VERSION == COD2_1_2
	int info_ping_offset = 0x6E6B4;
#elif COD_VERSION == COD2_1_3
	int info_ping_offset = 0x6E6B4;
#endif

	int ping = *(unsigned int *)(PLAYERBASE(id) + info_ping_offset);
	stackPushInt(ping);
}

void gsc_player_ClientCommand(int id)
{
	ClientCommand(id);
	stackPushInt(1);
}

void gsc_player_getLastConnectTime(int id)
{
#if COD_VERSION == COD2_1_0
	int info_connecttime_offset = 0x20D14;
#elif COD_VERSION == COD2_1_2
	int info_connecttime_offset = 0x20E24;
#elif COD_VERSION == COD2_1_3
	int info_connecttime_offset = 0x20E24;
#endif

	int lastconnect = getSVSTime() - *(unsigned int *)(PLAYERBASE(id) + info_connecttime_offset);
	stackPushInt(lastconnect);
}

int getLastPacketTime(int id)
{
#if COD_VERSION == COD2_1_0
	int info_lastmsg_offset = 0x20D10;
#elif COD_VERSION == COD2_1_2
	int info_lastmsg_offset = 0x20E20;
#elif COD_VERSION == COD2_1_3
	int info_lastmsg_offset = 0x20E20;
#endif

	return (PLAYERBASE(id) + info_lastmsg_offset);
}

void gsc_player_getLastMSG(int id)
{
	int lastmsg = getSVSTime() - *(unsigned int *)getLastPacketTime(id);
	stackPushInt(lastmsg);
}

void gsc_player_getclientstate(int id)
{
	stackPushInt(CLIENTSTATE(id));
}

void gsc_player_addresstype(int id)
{
	stackPushInt(ADDRESSTYPE(id));
}

void gsc_player_renameclient(int id)
{
	char * key;

	if ( ! stackGetParams("s", &key))
	{
		stackError("gsc_utils_execute() argument is undefined or has a wrong type");
		stackPushUndefined();
		return;
	}

	int info_player = PLAYERBASE(id);
	Info_SetValueForKey((char *)(info_player + 12), "name", key);
	char * name = (char *)(info_player + 134216);
	memcpy(&name[0], key, 32);
	name[31] = '\0';

	stackPushInt(1);
}

void gsc_player_outofbandprint(int id)
{
	char* cmd; // print\ninsert test message here!!!\n

	if ( ! stackGetParams("s", &cmd))
	{
		stackError("gsc_player_outofbandprint() argument is undefined or has a wrong type");
		stackPushUndefined();
		return;
	}

#if COD_VERSION == COD2_1_0
	int remoteaddress_offset = 452036;
#else
	int remoteaddress_offset = 452308;
#endif

	int info_player = PLAYERBASE(id);
	netadr_t * from = (netadr_t*)(info_player + remoteaddress_offset);
	NET_OutOfBandPrint(NS_SERVER, *from, cmd);

	stackPushInt(1);
}

void gsc_player_connectionlesspacket(int id)
{
	char* cmd; // rcon pass status

	if ( ! stackGetParams("s", &cmd))
	{
		stackError("gsc_player_connectionlesspacket() argument is undefined or has a wrong type");
		stackPushUndefined();
		return;
	}

	char message[COD2_MAX_STRINGLENGTH];
	message[0] = -1;
	message[1] = -1;
	message[2] = -1;
	message[3] = -1;
	message[4] = 0;
	strcat(message, cmd);
	msg_t msg;
	msg.data = message;
	msg.maxsize = 131072;
	msg.cursize = strlen(msg.data)+1;
	msg.readcount = 0;
	msg.overflowed = false;
	msg.bit = 0;

#if COD_VERSION == COD2_1_0
	int remoteaddress_offset = 452036;
#else
	int remoteaddress_offset = 452308;
#endif

	int info_player = PLAYERBASE(id);
	netadr_t * from = (netadr_t*)(info_player + remoteaddress_offset);
	SV_ConnectionlessPacket(*from, &msg);

	stackPushInt(1);
}

void gsc_player_resetNextReliableTime(int id)
{
#if COD_VERSION == COD2_1_0
	int offset = 134412;
#else
	int offset = 134684;
#endif

	*(int *)(PLAYERBASE(id) + offset) = 0;
	stackPushInt(0);
}

void gsc_player_ismantling(int id)
{
	int flags = PLAYERSTATE(id) + 0xc;

	if(*(int*)flags & 4) //oi m8 im mantling
		stackPushInt(1);
	else
		stackPushInt(0);
}

void gsc_player_isonladder(int id)
{
	int flags = PLAYERSTATE(id) + 0xc;

	if(*(int*)flags & 32)
		stackPushInt(1);
	else
		stackPushInt(0);
}

void gsc_player_getjumpslowdowntimer(int id)
{
	int value = PLAYERSTATE(id) + 16;
	stackPushInt(*(int*)value);
}

float player_movespeedscale[64] = {0};
int player_g_speed[64] = {0};
int player_g_gravity[64] = {0};

long double hook_player_setmovespeed(int client, int a2)
{
	float speed = calc_player_speed(client, a2);
	int id = PLAYERSTATE_ID(*(int*)client);

	if(speed > 0 && player_movespeedscale[id] > 0 && player_movespeedscale[id] != 1)
		return speed * player_movespeedscale[id];
	else
		return speed;
}

void gsc_player_setmovespeedscale(int id)
{
	float scale;
	if ( ! stackGetParams("f", &scale))
	{
		stackError("gsc_player_setmovespeedscale() argument is undefined or has a wrong type");
		stackPushUndefined();
		return;
	}

	if (scale <= 0)
	{
		stackError("gsc_player_setmovespeedscale() param must be above zero");
		stackPushUndefined();
		return;
	}

	player_movespeedscale[id] = scale;
	stackPushInt(1);
}

void gsc_player_setg_gravity(int id)
{
	int gravity;
	if ( ! stackGetParams("i", &gravity))
	{
		stackError("gsc_player_setg_gravity() argument is undefined or has a wrong type");
		stackPushUndefined();
		return;
	}

	if (gravity < 0)
	{
		stackError("gsc_player_setg_gravity() param must be equal or above zero");
		stackPushUndefined();
		return;
	}

	player_g_gravity[id] = gravity;
	stackPushInt(1);
}

void gsc_player_setg_speed(int id)
{
	int speed;
	if ( ! stackGetParams("i", &speed))
	{
		stackError("gsc_player_setg_speed() argument is undefined or has a wrong type");
		stackPushUndefined();
		return;
	}

	if (speed < 0)
	{
		stackError("gsc_player_setg_speed() param must be equal or above zero");
		stackPushUndefined();
		return;
	}

	player_g_speed[id] = speed;
	stackPushInt(1);
}

void hook_player_g_speed(int client)
{
	int player = *(int *)(client + 344);
	int id = G_ENTITY_ID(client);

	int newgravity = player_g_gravity[id];
	if(newgravity > 0)
		*(int *)(player + 72) = newgravity;

	int newspeed = player_g_speed[id];
	if(newspeed > 0)
		*(int *)(player + 80) = newspeed;

	calc_client_speed(client);
}

void gsc_player_setweaponfiremeleedelay(int id)
{
	int delay;
	if ( ! stackGetParams("i", &delay))
	{
		stackError("gsc_player_setweaponfiremeleedelay() argument is undefined or has a wrong type");
		stackPushUndefined();
		return;
	}

	if(delay < 0)
	{
		stackError("gsc_player_setweaponfiremeleedelay() param must be equal or above zero");
		stackPushUndefined();
		return;
	}

	int state = PLAYERSTATE(id);
	int* weapondelay = (int *)(state + 0x34);
	*weapondelay = delay;
	*(int *)(state + 216) = 11;
}

int disable_player_item_pickup[64] = {0};

int hook_pickup_item(int weapon, int player, int message)
{
	int clientNum = G_ENTITY_ID(player);
	if(disable_player_item_pickup[clientNum] != 1)
	{
		typedef int (*Touch_Item_Auto_t)(int a1, int a2, int a3);
#if COD_VERSION == COD2_1_0
		Touch_Item_Auto_t Touch_Item_Auto = (Touch_Item_Auto_t)0x081037F0;
#elif COD_VERSION == COD2_1_2
		Touch_Item_Auto_t Touch_Item_Auto = (Touch_Item_Auto_t)0x08105B24;
#elif COD_VERSION == COD2_1_3
		Touch_Item_Auto_t Touch_Item_Auto = (Touch_Item_Auto_t)0x08105C80;
#else
		Touch_Item_Auto_t Touch_Item_Auto = (Touch_Item_Auto_t)NULL;
#endif
		return Touch_Item_Auto(weapon, player, message);
	}

	return 1;
}

void gsc_player_disable_item_pickup(int id)
{
	disable_player_item_pickup[id] = 1;
}

void gsc_player_enable_item_pickup(int id)
{
	disable_player_item_pickup[id] = 0;
}

void gsc_player_set_anim(int id)
{
	char* animation;

	if ( ! stackGetParams("s", &animation))
	{
		stackError("gsc_player_set_anim() argument is undefined or has a wrong type");
		stackPushUndefined();
		return;
	}

#if COD_VERSION == COD2_1_0
	int anim_offset = 0x080D46AC;
#elif COD_VERSION == COD2_1_2
	int anim_offset = 0x080D6C8C;
#elif COD_VERSION == COD2_1_3
	int anim_offset = 0x080D6DD0;
#endif

	int (*BG_AnimationIndexForString)(char *src);
	*(int *)&BG_AnimationIndexForString = anim_offset;

	int animationIndex = 0;
	extern int custom_animation[64];

	if(strcmp(animation, "none"))
		animationIndex = BG_AnimationIndexForString(animation);

	custom_animation[id] = (animationIndex);
}

#if COMPILE_BOTS == 1
void gsc_player_set_walkdir(int id)
{
	char* dir;

	if ( ! stackGetParams("s", &dir))
	{
		stackError("gsc_player_set_walkdir() argument is undefined or has a wrong type");
		stackPushUndefined();
		return;
	}

	extern int bot_movement[64];
	if(!strcmp(dir, "none"))
		bot_movement[id] = (0);
	else if(!strcmp(dir, "forward"))
		bot_movement[id] = (127);
	else if(!strcmp(dir, "back"))
		bot_movement[id] = (129);
	else if(!strcmp(dir, "right"))
		bot_movement[id] = (127 << 8);
	else if(!strcmp(dir, "left"))
		bot_movement[id] = (129 << 8);
}

void gsc_player_set_lean(int id)
{
	char* lean;

	if ( ! stackGetParams("s", &lean))
	{
		stackError("gsc_player_set_lean() argument is undefined or has a wrong type");
		stackPushUndefined();
		return;
	}

	extern int bot_lean[64];
	if(!strcmp(lean, "none"))
		bot_lean[id] = (0);
	else if(!strcmp(lean, "left"))
		bot_lean[id] = (64);
	else if(!strcmp(lean, "right"))
		bot_lean[id] = (128);
}

void gsc_player_set_stance(int id)
{
	char* stance;

	if ( ! stackGetParams("s", &stance))
	{
		stackError("gsc_player_set_stance() argument is undefined or has a wrong type");
		stackPushUndefined();
		return;
	}

	extern int bot_stance[64];
	if(!strcmp(stance, "stand"))
		bot_stance[id] = (0);
	else if(!strcmp(stance, "crouch"))
		bot_stance[id] = (512);
	else if(!strcmp(stance, "prone"))
		bot_stance[id] = (256);
	else if(!strcmp(stance, "jump"))
		bot_stance[id] = (1024);
}

void gsc_player_thrownade(int id)
{
	int grenade;

	if ( ! stackGetParams("i", &grenade))
	{
		stackError("gsc_player_thrownade() argument is undefined or has a wrong type");
		stackPushUndefined();
		return;
	}

	extern int bot_grenade[64];
	if (!grenade)
		bot_grenade[id] = (0);
	else
		bot_grenade[id] = (65536);
}

void gsc_player_fireweapon(int id)
{
	int shoot;

	if ( ! stackGetParams("i", &shoot))
	{
		stackError("gsc_player_fireweapon() argument is undefined or has a wrong type");
		stackPushUndefined();
		return;
	}

	extern int bot_shoot[64];
	if (!shoot)
		bot_shoot[id] = (0);
	else
		bot_shoot[id] = (1);
}

void gsc_player_meleeweapon(int id)
{
	int melee;

	if ( ! stackGetParams("i", &melee))
	{
		stackError("gsc_player_meleeweapon() argument is undefined or has a wrong type");
		stackPushUndefined();
		return;
	}

	extern int bot_melee[64];
	if (!melee)
		bot_melee[id] = (0);
	else
		bot_melee[id] = (32772);
}

void gsc_player_reloadweapon(int id)
{
	int reload;

	if ( ! stackGetParams("i", &reload))
	{
		stackError("gsc_player_reloadweapon() argument is undefined or has a wrong type");
		stackPushUndefined();
		return;
	}

	extern int bot_reload[64];
	if (!reload)
		bot_reload[id] = (0);
	else
		bot_reload[id] = (16);
}

void gsc_player_adsaim(int id)
{
	int ads;

	if ( ! stackGetParams("i", &ads))
	{
		stackError("gsc_player_adsaim() argument is undefined or has a wrong type");
		stackPushUndefined();
		return;
	}

	extern int bot_ads[64];
	if (!ads)
		bot_ads[id] = (0);
	else
		bot_ads[id] = (4096);
}

void gsc_player_switchtoweaponid(int id)
{
	int weaponid;

	if ( ! stackGetParams("i", &weaponid))
	{
		stackError("gsc_player_switchtoweaponid() argument is undefined or has a wrong type");
		stackPushUndefined();
		return;
	}

	extern int bot_weapon[64];
	bot_weapon[id] = (weaponid);
}
#endif

void gsc_player_getcooktime(int id)
{
	int nadetime = PLAYERSTATE(id) + 60;

	if(*(int*)nadetime)
		stackPushInt(*(int*)nadetime);
	else
		stackPushInt(0);
}

// entity functions (could be in own file, but atm not many pure entity functions)
void gsc_entity_setalive(int id)   // as in isAlive?
{
	int isAlive;

	if ( ! stackGetParams("i", &isAlive))
	{
		stackError("gsc_entity_setalive() argument is undefined or has a wrong type");
		stackPushUndefined();
		return;
	}

	*(char *)(G_ENTITY(id) + 353) = isAlive;
	stackPushInt(1);
}

void gsc_entity_setbounds(int id)
{
	float width, height;

	if ( ! stackGetParams("ff", &width, &height))
	{
		stackError("gsc_entity_setbounds() one or more arguments is undefined or has a wrong type");
		stackPushUndefined();
		return;
	}

	*(float*)(G_ENTITY(id) + 280) = height;
	*(float*)(G_ENTITY(id) + 276) = width;
	*(float*)(G_ENTITY(id) + 272) = width;
	*(float*)(G_ENTITY(id) + 264) = -width;
	*(float*)(G_ENTITY(id) + 260) = -width;

	stackPushInt(1);
}

void gsc_kick_slot()
{
	int id;
	char* msg;

	if ( ! stackGetParams("is", &id, &msg))
	{
		stackError("gsc_kick_slot() one or more arguments is undefined or has a wrong type");
		stackPushUndefined();
		return;
	}

	if (ADDRESSTYPE(id) == NA_LOOPBACK)
	{
		stackPushInt(0);
		return; // host
	}

	int entity = PLAYERBASE(id);

	SV_DropClient(entity, msg);
	stackPushInt(1);
}

void gsc_player_setguid(int id)
{
	int guid;

	if ( ! stackGetParams("i", &guid))
	{
		stackError("gsc_player_setguid() argument is undefined or has a wrong type");
		stackPushUndefined();
		return;
	}

#if COD_VERSION == COD2_1_0
	int guid_offset = 0x765F4;
#elif COD_VERSION == COD2_1_2
	int guid_offset = 0x76704;
#elif COD_VERSION == COD2_1_3
	int guid_offset = 0xAE704;
#endif

	int cl = PLAYERBASE(id);
	*(int*)(cl + guid_offset) = guid;
	stackPushInt(1);
}

void gsc_player_clienthasclientmuted(int id)
{
	int id2;

	if ( ! stackGetParams("i", &id2))
	{
		stackError("gsc_player_clienthasclientmuted() argument is undefined or has a wrong type");
		stackPushUndefined();
		return;
	}

	stackPushInt(SV_ClientHasClientMuted(id, id2));
}

void gsc_player_getlastgamestatesize(int id)
{
	extern int gamestate_size[64];
	stackPushInt(gamestate_size[id]);
}

void gsc_player_getfps(int id)
{
	extern int clientfps[64];
	stackPushInt(clientfps[id]);
}

#endif
