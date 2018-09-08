#ifndef Enum_h__
#define Enum_h__

namespace Player
{
	typedef enum PlayerAnimation
	{
		battle_attack_01_a,	//1Ÿ
		battle_attack_01_b,	//ó��(���Ž�2) �� ��Ÿ ���� ��Ÿ1 �� ���Ǵ� ��Ÿ
		battle_attack_02,	//2Ÿ
		battle_attack_03,	//3Ÿ
		battle_attack_04,	//4Ÿ 4

		battle_attack_break_l,	//����
		battle_attack_break_r,	//����
		battle_attack_break_speedymove_l,	//��Ÿ� ���� ����
		battle_attack_break_speedymove_r,	//��Ÿ� ���� ���� 8

		battle_attack_parry_left_l,	//���� ª�� �ĳ���
		battle_attack_parry_left_r,	//���� ��� �ĳ���
		battle_attack_parry_right_l,//������ ��� �ĳ���
		battle_attack_parry_right_r,//������ ��� �ĳ���
		battle_attack_parry_up_l,	//���� �� �ĳ���
		battle_attack_parry_up_r,	//������ �� �ĳ��� 14

		battle_attack_run_sprint,	//�޸��ٰ� �� �����̷� ġ��

		battle_attack_run_sprint_power_begin,	//ȸ���� ����
		battle_attack_run_sprint_power_during_a,//ȸ����
		battle_attack_run_sprint_power_during_b,//ȸ����(a�� ����)
		battle_attack_run_sprint_power_end,		//ȸ���� ������

		battle_attack_strong_00,		//�߷� ���鰭Ÿ(ª�� ������) => �����
		battle_attack_strong_00_charge, //�߷� ���鰭Ÿ(��� ������) => ��� ����

		battle_attack_strong_01,		//ù��° ���Ž�

		battle_attack_strong_02_a,		//�ι�° ���Ž� ����(ó��)
		battle_attack_strong_02_b,		//ó�� �÷��� ���
		battle_attack_strong_02_b_reverse, //�ݴ������� ó�� �÷��� ���

		battle_attack_strong_02_c_1,		//���� �������� ó�� �÷��� ���
		battle_attack_strong_02_c_2,		//���� �������� ó�� �÷��� ��� 2��°
		battle_attack_strong_02_c_final,	//ó�� ������

		battle_attack_strong_02_c_reverse_1,	//���� ���� �ݴ� ó�� �÷��� ���
		battle_attack_strong_02_c_reverse_2,	//���� ���� �ݴ� ó�� �÷��� ��� 2��°
		battle_attack_strong_02_c_reverse_final,//���� ���� �ݴ� ó�� ������

		battle_attack_strong_03,				//����° ��Ÿ �� ���Ž�

		battle_attack_strong_04_a,				//4��° ��Ÿ�� ���Ž�
		battle_attack_strong_04_b,				//4��° ��Ÿ�� ���Ž� ���� ���Ž�

		battle_attack_strong_run_sprint,		//������ ó��

		battle_idle_lower,						//���� ������
		battle_idle2rest,						//���� ��⿡�� �޽����� �Ѿ�� �ִϸ��̼�
		battle_rest,							//�޽�4

		battle_run_lower,		//õõ�� �޸���
		battle_run_push,		//�б�
		battle_run_sprint,		//���� �޸���
		battle_run_sprint_stop, //Ǯ�� �޸��ٰ� ���߱�
		battle_run_stop,		//���� �޸��ٰ� ���߱�

		kick,					//������

		SP_attack_charge_slash_begin_l, //������ ��¡ ����
		SP_attack_charge_slash_begin_r, //���� ��¡ ����
		SP_attack_charge_slash_during_l,//��������¡��
		SP_attack_charge_slash_during_r,//������¡��

		SP_attack_charge_slash_left_l,
		SP_attack_charge_slash_left_r,
		SP_attack_charge_slash_right_l,
		SP_attack_charge_slash_right_r,

		SP_attack_charge_slash_success, //������ ��¡�ÿ��� ����(�������� ��¡�ؼ� �������Ŀ��� �ٷ� ��� �������� ����)
		SP_attack_charge_slash_up_l,	//���ʹ� ���� ��¡ ����
		SP_attack_charge_slash_up_r,	//�����ʹ� ���� ��¡ ����

		SP_attack_execution,		//�ʻ��� �ϰ�
		SP_attack_hell,				//����		wasddwsadwsaddddddw
		SP_attack_hell_fail,		//������ �ƹ��͵� Ÿ���þȵǾ����� ����
		SP_attack_hell_human,		//����(�ΰ�������?)
		SP_attack_judgement_l,		//ó�� ����
		SP_attack_judgement_r,		//�������� �� ����
		SP_cast_berserker,			//����ȭ ��ų ���

		speedymove_l,				//���� �� ���϶� �޸��鼭 Space(����)
		speedymove_r,				//������ �� ���϶� �޸��鼭 Space(����)
		steel_parry_up_l,			//battle_attack_parry_up_l,	//���� �� �ĳ��� ������ �ĳ��� �Ҷ� ����ϴµ�
		steel_parry_up_r,			//battle_attack_parry_up_r,	//������ �� �ĳ���

		damage_flinch,
		damage_launch_begin,
		damage_launch_down,
		damage_launch_during,
		damage_launch_end,
		damage_launch_strong,
		damage_launch_weak,

		damage_light_back,
		damage_light_front,
		damage_light_left,
		damage_light_right,
		damage_light_up,

		damage_middle_back_begin,
		damage_middle_back_during,	//�ʹ� �� begin ���� �ٷ� end�� �Ѿ�� ��
		damage_middle_back_end,

		damage_middle_front_begin,
		damage_middle_front_during, //�ʹ� �� begin ���� �ٷ� end�� �Ѿ�� ��
		damage_middle_front_end,

		damage_strong_back_begin,
		damage_strong_front_begin,

		down_back_idle,			//�̻��
		down_back_roll_front,	//�ٿ�� ���¿��� Ű�� ������ ���	
		down_back2idle,			//����� ���

		down_front_idle,		//�̻��
		down_front_roll_front,	////�ٿ�� ���¿��� Ű�� ������ ���
		down_front2idle,		//����� ���

		down_resist,			//Ȯ�������� ��Ƽ���Ѵ�.

		down_safefall,			//�̻��
		down_safefall_backroll, //�̻��
		down_safefall_frontroll,//�̻��

		tired_idle_begin,
		tired_idle_during,
		tired_idle_end,
		tired_run,

		dark_knight_2_skill_tentacle,
		dark_knight_transformation_begin_1,
		dark_knight_transformation_begin_2,
		dark_knight_transformation_end_1,

		potion_begin,
		potion_during,
		potion_end,
		potion_run,

		repair_begin,
		repair_during,
		repair_end,

		No_Mation,
		PlayerAnimation_End,
	}PLAYERANIMATION;
}

namespace BloodLord
{
	typedef enum BloodLordAnimation
	{
		blind_begin,	// ����ε�? ���� ����ϴ��� ���� ��
		blind_during,
		blind_end,

		damage_weak_back,	// �ǰ� ���
		damage_weak_bottom,
		damage_weak_left,
		damage_weak_right,
		damage_weak_up,

		dominated_begin,
		dominated_during,
		dominated_end,

		dying_from_back,	// �״� ���
		dying_from_front,
		dying_from_left,
		dying_from_right,

		falling,
		idle,	
		run_E,		// �ٱ�
		run_N,
		run_S,
		run_W,

		turn_left,	// �������� ȸ��
		turn_right,	// ���������� ȸ��

		walk_E,		// �ȱ�
		walk_N,
		walk_S,
		walk_W,

		vamplord_idle_low_health,	// �� �����Ҷ� ���
		weakpoint_during,

		vamplord_cutscene_1,	// ���� �ƽ� 1
		vamplord_cutscene_2,	// ���� �ƽ� 2

		////attack motion

		vamplord_attack_blood_breath,		// �극�� ����( �� ������ )

		vamplord_attack_bloodfield_begin1,	// ���׿�?
		vamplord_attack_bloodfield_begin2,
		vamplord_attack_bloodfield_during1,
		vamplord_attack_bloodfield_during2,
		vamplord_attack_bloodfield_end,

		vamplord_attack_dancing_begin,		// ���߿� ���� ���ۺ��� ���鼭 ����
		vamplord_attack_dancing_during,
		vamplord_attack_dancing_end,
		vamplord_attack_dancing_short_begin,
		vamplord_attack_dancing_short_end,

		vamplord_attack_deadclaw_begin,		// �۰� �� ���ø��� ����
		vamplord_attack_deadclaw_fail,
		vamplord_attack_deadclaw_pierce,
		vamplord_attack_deadclaw_success_begin,
		vamplord_attack_deadclaw_success_during,
		vamplord_attack_deadclaw_success_end,

		vamplord_attack_flash,
		vamplord_attack_flash_begin,

		vamplord_attack_forehand_hard,	// ��ġ��
		vamplord_attack_hard,			// ���� 2Ÿ
		vamplord_attack_hello,			// ������ �帱ó�� ���鼭 ����

		vamplord_attack_summon_begin,	// ���׿��� ����ѵ� �� ������ ����
		vamplord_attack_summon_during,

		vamplord_attack_turnleft2_hard,
		vamplord_attack_turrightt2_hard,

		vamplord_attack02_begin_hard,	//
		vamplord_attack02_end_hard,
		vamplord_attack02_hard,			// �Ѽ�ġ��

		vamplord_attack03_fast_hard,	// �а� �����̸鼭 5Ÿ
		vamplord_attack03_hard,			// ���ڸ����� 5Ÿ

		vamplord_laugh,

		vamplord_swamp_begin,	// �ǿ����� ���� ����
		vamplord_swamp_during,
		vamplord_swamp_end,
		vamplord_swamp_ready,

		vamplord_threat,

		vamplord_wrap_air_hard,		
		vamplord_wrap_air_hard_end,	// ��ũ ��

		vamplord_wrap_air2_begin,
		vamplord_wrap_air2_during,
		vamplord_wrap_air2_end,		// ��ũ �� ����

		vamplord_wrap_attack_hard,
		vamplord_wrap_hard,

		////downknee motion
		down_from_back2downback,
		down_from_bottom2downfront,
		down_from_front2downfront,
		down_from_left2downback,
		down_from_right2downfront,
		down_from_up2downback,

		down_idle_facedown,
		down_idle_faceup,

		standup_from_facedown,
		standup_from_faceup,

		////duel motion
		duel_advantage,
		duel_disadvantage,
		duel_idle,
		duel_lose_damage,	//�ĳ��� �������� ���
		duel_lose_wait,
		duel_vibrate,
		duel_win,

		spawn,

		BloodLordAnimation_End,
	}BLOODLORDANIMATION;
}

namespace Glitter
{
	typedef enum GlitterAnimation
	{
		Spawn,
		idle,
		turn_Left,
		turn_Right,
		WalkE,
		WalkW,
		WalkS,
		WalkN,
		RunN,
		vampire_attack_lightning_onehand_glitter,
		vampire_attack_lightning_strike,
		vampire_attack_lightning_around,
		vampire_attack_forehand2_lightning,
		down_start,
		down_idle,
		down_standup,
		down_from_front2downback,
		down_from_bottom2downfront,
		damage_weak_back,
		damage_weak_front,
		GlitterAnimation_End,

	}GLITTERANIMATION;
}

namespace Moonchopper
{
	typedef enum MoonchopperAnimation
	{
		spawn,
		idle,
		walkE,
		walkW,
		walkS,
		walkN,
		runN,
		turn_Right,
		turn_Left,
		upper,
		vampire_attack_pierceaura_begin,
		vampire_attack_pierceaura_end,
		vampire_attack_doublepierce,
		vampire_attack_slash_and_pierce,
		vampire_attack_pierece_and_spin,
		low_health,
		down_from_right2downback,
		damage_weak_back,
		damage_weak_front,
		MoonchopperAnimation_End,

	}MOONCHOPPERANIMATION;
}


namespace BloodJelly
{
	typedef enum BloodJellyAnimation
	{
		spawn,
		idle,
		runE,
		runW,
		runS,
		runN,
		jelly_attack_explosion,
		jelly_attack_swoop,
		jelly_damage_weak_back,
		jelly_damage_weak_bottom,
		jelly_damage_dying_bottom,

		BloodJelly_end,
	}BLOODJELLYANIMATION;
}
namespace RoyalGuard
{
	typedef enum RoyalGuardAnimation
	{
		spawn,
		idle,
		walkE,
		walkW,
		walkS,
		walkN,
		runN,
		vampire_attack_double,
		vampire_attack_pierce,
		vampire_damage_weak_back,
		vampire_damage_weak_bottom,
		down,

		RoyalGuard_end,
	}ROYALGUARDANIMATION;
}
enum BOXTAG
{
	BOX_HEAD,
	BOX_UPPER,
	BOX_LOWER,
	BOX_HAND,
	BOX_FOOT,
	BOX_DAMAGE,
	BOX_ATTACK,

	BOX_END
};

enum CameraModeType
{
	FREEMODE,
	TARGETMODE,
	OBSERVERMODE,
	CMT_END,
};

enum StandardType
{
	LASTCP,
	SPECIFIC,
	ST_END,
};

enum DamageDirectionType
{
	DDT_FRONT,		//����
	DDT_BACK,		//�Ĺ�
	DDT_FRONT_LEFT,		//���� ����
	DDT_BACK_LEFT,		//�Ĺ� ����
	DDT_FRONT_RIGHT,	//���� ����
	DDT_BACK_RIGHT,		//�Ĺ� ����
};

enum DFONT_FONT_TYPE	// ��Ʈ�� ����
{ 
	DFONT_TYPE_DAMAGE,		// ������ ��Ʈ�� ����Ҷ� 
	DFONT_TYPE_OVERKILL,	// ����ų ��Ʈ�� ����Ҷ�
	DFONT_TYPE_END
};	// ��Ʈ�� ������ ����

enum DFONT_OBJ_TYPE			// �������� �޴� ��ü
{ 
	DFONT_OBJ_NULL,			// �ʱⰪ ���� / ����ų�϶� / ������ �ȳְ� ������
	DFONT_OBJ_PLAYER,		// �÷��̾ �ǰݴ��Ҷ�
	DFONT_OBJ_MONSTER ,		// ���Ͱ� �ǰݴ��Ҷ�
	DFONT_STATE_END
};	// ������ ��Ʈ�� ���¸� ����

namespace SOUND
{
	typedef enum Bgm
	{
		BGM_BloodLord,
		BGM_StartScene,
		BGM_MiddleStage,
		BGM_SoundEnd
	}BGM;

	typedef enum Effect
	{
		//Player
		hurk_attack_01,
		hurk_attack_02,
		hurk_attack_03,
		hurk_attack_04,
		hurk_attack_05,
		hurk_attack_06,
		hurk_attack_07,

		hurk_attack_long_01,
		hurk_attack_long_02,
		hurk_attack_long_03,
		hurk_attack_long_04,

		hurk_attack_skill_blunt_01,
		hurk_attack_skill_blunt_02,
		hurk_attack_skill_blunt_03,
		hurk_attack_skill_blunt_04,

		hurk_attack_skill_execution_01,
		hurk_attack_skill_execution_02,

		hurk_attack_skill_hell_01,
		hurk_attack_skill_hell_02,
		hurk_attack_skill_hell_03,
		hurk_attack_skill_hell_04,

		hurk_attack_skill_hell_finish_01,
		hurk_attack_skill_hell_finish_02,

		hurk_attack_skill_parrying_cast_01,
		hurk_attack_skill_parrying_cast_02,
		hurk_attack_skill_parrying_cast_03,
		hurk_attack_skill_parrying_cast_04,
		hurk_attack_skill_parrying_cast_05,

		hurk_attack_skill_piercing_last_01,
		hurk_attack_skill_piercing_last_02,
		hurk_attack_skill_piercing_last_03,

		hurk_attack_skill_swingstrong1_01,
		hurk_attack_skill_swingstrong1_02,
		hurk_attack_skill_swingstrong1_03,
		hurk_attack_skill_swingstrong1_04,

		hurk_attack_skill_swingstrong2_01,
		hurk_attack_skill_swingstrong2_02,
		hurk_attack_skill_swingstrong2_03,
		hurk_attack_skill_swingstrong2_04,

		hurk_attack_strong_01,
		hurk_attack_strong_02,
		hurk_attack_strong_03,
		hurk_attack_strong_04,
		hurk_attack_strong_05,
		hurk_attack_strong_06,
		hurk_attack_strong_07,
		hurk_attack_strong_08,
		hurk_attack_strong_09,
		hurk_attack_strong_10,

		hurk_hurt_medium_1,
		hurk_hurt_medium_2,
		hurk_hurt_medium_3,
		hurk_hurt_medium_4,
		hurk_hurt_medium_5,
		hurk_hurt_medium_6,
		hurk_hurt_medium_7,

		hurk_hurt_strong_01,
		hurk_hurt_strong_02,
		hurk_hurt_strong_03,
		hurk_hurt_strong_04,
		hurk_hurt_strong_05,
		hurk_hurt_strong_06,
		hurk_hurt_strong_07,
		hurk_hurt_strong_08,

		hurk_hurt_weak_01,
		hurk_hurt_weak_02,
		hurk_hurt_weak_03,
		hurk_hurt_weak_04,
		hurk_hurt_weak_05,
		hurk_hurt_weak_06,

		step_lv3,
		step_lv4,

		Hurk_Swing_GreatSword_01,
		Hurk_Swing_GreatSword_AttackBreak_01,
		Hurk_Swing_GreatSword_Blow_01,
		Hurk_Swing_GreatSword_Execution_01,
		Hurk_Swing_GreatSword_Friction_01,
		Hurk_Swing_GreatSword_Hell_01,
		Hurk_Swing_GreatSword_KnockBack_01,
		Hurk_Swing_GreatSword_Parrying_Blunt_01,
		Hurk_Swing_GreatSword_Parrying_Metal_01,
		Hurk_Swing_GreatSword_Piercing_01,
		Hurk_Swing_GreatSword_SmashCombo_01,
		Hurk_Swing_GreatSword_Strong_01,
		Hurk_Swing_GreatSword_Strong_02,

		material_dirt_step1,
		material_dirt_step2,
		material_dirt_step3,
		material_dirt_step4,

		heart_beat,

		//BloodLord
		lord_attack01,
		lord_attack02,
		lord_attack03,
		lord_bone01,
		lord_bone02,
		lord_bone03,
		lord_bone04,
		lord_breath01,
		lord_breath02,
		lord_breath03,
		lord_dominate,
		lord_dying,
		lord_dying2,
		lord_firebreath,
		lord_low_health,
		lord_roar,
		lord_spawn,
		lord_threat,
		lord_warp_begin,
		lord_warp_end,
		explosion_lv4,

		//Vampire
		action_landing_higher,
		Bear_Step,
		effect_fireball_appear,
		effect_lightning_charge,
		effect_lightning_explosion,
		effect_lightning_shock,
		vampire_attack_drag,
		vampire_breath1,
		vampire_breath2,
		vampire_breath3,
		vampire_dying,
		vampire_fireblade_begin,
		vampire_fireblade_during,
		vampire_fireblade_end,
		vampire_fireblade_original,
		vampire_hurt1,
		vampire_hurt2,
		vampire_hurt3,
		vampire_laugh1,
		vampire_laugh2,
		vampire_laugh3,
		vampire_pierceaura_begin,
		vampire_pierceaura_end,
		vampire_rapier_pierce,
		vampire_rapier_vanguard,
		vampire_shout_long1,
		vampire_shout_long2,
		vampire_shout_long3,
		vampire_shout1,
		vampire_shout2,
		vampire_shout3,
		vampire_standup1,
		vampire_standup2,
		vampire_step1,
		vampire_step2,
		vampire_step3,
		vampire_step4,
		Swing_Metal,
		Swing_MetalStrong,

		//Hit
		//Player
		Hit_Metal_Slash,
		Hit_Metal_Slash_adult,
		Hit_Metal_Stab,
		Hit_Metal_Stab_adult,
		Hit_Metal_StrongSlash,
		Hit_Metal_StrongSlash_adult,

		Hit_Flesh_Blunt,
		Hit_Flesh_Blunt_adult,

		//Monster
		Hit_Flesh_Slash,
		Hit_Flesh_Slash_adult,
		Hit_Flesh_Stab,
		Hit_Flesh_Stab_adult,
		Hit_HardFlesh_Slash,
		Hit_HardFlesh_Slash_adult,
		Hit_HardFlesh_Slash2,

		//���� ����Ʈ
		BurningFire,
		octopus_drill_wood_01,
		octopus_drill_wood_02,
		octopus_drill_wood_03,
		Click,
		material_rock_break,

		//Jelly
		jelly_attack_explosion,
		jelly_attack1,
		jelly_damage_strong1,
		jelly_damage_strong2,
		jelly_down,
		jelly_idle,
		jelly_pierce,
		jelly_walk1,
		jelly_walk2,
		jelly_weak1,
		jelly_weak2,

		EFF_SoundEnd
	}EFF;
};
#endif // Enum_h__

