#ifndef Enum_h__
#define Enum_h__

namespace Player
{
	typedef enum PlayerAnimation
	{
		battle_attack_01_a,	//1타
		battle_attack_01_b,	//처형(스매시2) → 평타 사용시 평타1 때 사용되는 평타
		battle_attack_02,	//2타
		battle_attack_03,	//3타
		battle_attack_04,	//4타 4

		battle_attack_break_l,	//봉쇄
		battle_attack_break_r,	//봉쇄
		battle_attack_break_speedymove_l,	//긴거리 무브 봉쇄
		battle_attack_break_speedymove_r,	//긴거리 무브 봉쇄 8

		battle_attack_parry_left_l,	//왼쪽 짧게 쳐내기
		battle_attack_parry_left_r,	//왼쪽 길게 쳐내기
		battle_attack_parry_right_l,//오른쪽 길게 쳐내기
		battle_attack_parry_right_r,//오른쪽 길게 쳐내기
		battle_attack_parry_up_l,	//왼쪽 위 쳐내기
		battle_attack_parry_up_r,	//오른쪽 위 쳐내기 14

		battle_attack_run_sprint,	//달린다가 검 손잡이로 치기

		battle_attack_run_sprint_power_begin,	//회오리 시작
		battle_attack_run_sprint_power_during_a,//회오리
		battle_attack_run_sprint_power_during_b,//회오리(a와 같음)
		battle_attack_run_sprint_power_end,		//회오리 마무리

		battle_attack_strong_00,		//발로 지면강타(짧게 누르면) => 사용함
		battle_attack_strong_00_charge, //발로 지면강타(길게 누르면) => 사용 안함

		battle_attack_strong_01,		//첫번째 스매쉬

		battle_attack_strong_02_a,		//두번째 스매쉬 시작(처형)
		battle_attack_strong_02_b,		//처형 올려서 찍기
		battle_attack_strong_02_b_reverse, //반대쪽으로 처형 올려서 찍기

		battle_attack_strong_02_c_1,		//원래 방향으로 처형 올려서 찍기
		battle_attack_strong_02_c_2,		//원래 방향으로 처형 올려서 찍기 2번째
		battle_attack_strong_02_c_final,	//처형 마무리

		battle_attack_strong_02_c_reverse_1,	//원래 방향 반대 처형 올려서 찍기
		battle_attack_strong_02_c_reverse_2,	//원래 방향 반대 처형 올려서 찍기 2번째
		battle_attack_strong_02_c_reverse_final,//원래 방향 반대 처형 마무리

		battle_attack_strong_03,				//세번째 평타 후 스매쉬

		battle_attack_strong_04_a,				//4번째 평타후 스매쉬
		battle_attack_strong_04_b,				//4번째 평타후 스매쉬 연계 스매쉬

		battle_attack_strong_run_sprint,		//돌격후 처형

		battle_idle_lower,						//공격 대기상태
		battle_idle2rest,						//공격 대기에서 휴식으로 넘어가는 애니메이션
		battle_rest,							//휴식4

		battle_run_lower,		//천천히 달리기
		battle_run_push,		//밀기
		battle_run_sprint,		//빨리 달리기
		battle_run_sprint_stop, //풀로 달리다가 멈추기
		battle_run_stop,		//빨리 달리다가 멈추기

		kick,					//발차기

		SP_attack_charge_slash_begin_l, //오른쪽 차징 시작
		SP_attack_charge_slash_begin_r, //왼쪽 차징 시작
		SP_attack_charge_slash_during_l,//오른쪽차징중
		SP_attack_charge_slash_during_r,//왼쪽차징중

		SP_attack_charge_slash_left_l,
		SP_attack_charge_slash_left_r,
		SP_attack_charge_slash_right_l,
		SP_attack_charge_slash_right_r,

		SP_attack_charge_slash_success, //오른쪽 차징시에만 존재(왼쪽으로 차징해서 공격한후에는 바로 대기 동작으로 간다)
		SP_attack_charge_slash_up_l,	//왼쪽발 기준 차징 공격
		SP_attack_charge_slash_up_r,	//오른쪽발 기준 차징 공격

		SP_attack_execution,		//필살의 일격
		SP_attack_hell,				//절명		wasddwsadwsaddddddw
		SP_attack_hell_fail,		//절명후 아무것도 타켓팅안되어을때 실패
		SP_attack_hell_human,		//절명(인간형한테?)
		SP_attack_judgement_l,		//처음 심판
		SP_attack_judgement_r,		//연속으로 쓴 심판
		SP_cast_berserker,			//광폭화 스킬 모션

		speedymove_l,				//왼쪽 발 앞일때 달리면서 Space(돌격)
		speedymove_r,				//오른쪽 발 앞일때 달리면서 Space(돌격)
		steel_parry_up_l,			//battle_attack_parry_up_l,	//왼쪽 위 쳐내기 여러번 쳐내기 할때 사용하는듯
		steel_parry_up_r,			//battle_attack_parry_up_r,	//오른쪽 위 쳐내기

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
		damage_middle_back_during,	//너무 길어서 begin 에서 바로 end로 넘어가게 함
		damage_middle_back_end,

		damage_middle_front_begin,
		damage_middle_front_during, //너무 길어서 begin 에서 바로 end로 넘어가게 함
		damage_middle_front_end,

		damage_strong_back_begin,
		damage_strong_front_begin,

		down_back_idle,			//미사용
		down_back_roll_front,	//다운된 상태에서 키를 누르면 사용	
		down_back2idle,			//연계로 사용

		down_front_idle,		//미사용
		down_front_roll_front,	////다운된 상태에서 키를 누르면 사용
		down_front2idle,		//연계로 사용

		down_resist,			//확률적으로 버티게한다.

		down_safefall,			//미사용
		down_safefall_backroll, //미사용
		down_safefall_frontroll,//미사용

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
		blind_begin,	// 블라인드? 언제 사용하는지 아직 모름
		blind_during,
		blind_end,

		damage_weak_back,	// 피격 모션
		damage_weak_bottom,
		damage_weak_left,
		damage_weak_right,
		damage_weak_up,

		dominated_begin,
		dominated_during,
		dominated_end,

		dying_from_back,	// 죽는 모션
		dying_from_front,
		dying_from_left,
		dying_from_right,

		falling,
		idle,	
		run_E,		// 뛰기
		run_N,
		run_S,
		run_W,

		turn_left,	// 왼쪽으로 회전
		turn_right,	// 오른쪽으로 회전

		walk_E,		// 걷기
		walk_N,
		walk_S,
		walk_W,

		vamplord_idle_low_health,	// 피 부족할때 대기
		weakpoint_during,

		vamplord_cutscene_1,	// 등장 컷신 1
		vamplord_cutscene_2,	// 등장 컷신 2

		////attack motion

		vamplord_attack_blood_breath,		// 브레스 공격( 피 내뿜음 )

		vamplord_attack_bloodfield_begin1,	// 메테오?
		vamplord_attack_bloodfield_begin2,
		vamplord_attack_bloodfield_during1,
		vamplord_attack_bloodfield_during2,
		vamplord_attack_bloodfield_end,

		vamplord_attack_dancing_begin,		// 공중에 떠서 빙글빙글 돌면서 공격
		vamplord_attack_dancing_during,
		vamplord_attack_dancing_end,
		vamplord_attack_dancing_short_begin,
		vamplord_attack_dancing_short_end,

		vamplord_attack_deadclaw_begin,		// 송곳 찍어서 들어올리는 공격
		vamplord_attack_deadclaw_fail,
		vamplord_attack_deadclaw_pierce,
		vamplord_attack_deadclaw_success_begin,
		vamplord_attack_deadclaw_success_during,
		vamplord_attack_deadclaw_success_end,

		vamplord_attack_flash,
		vamplord_attack_flash_begin,

		vamplord_attack_forehand_hard,	// 덮치기
		vamplord_attack_hard,			// 연속 2타
		vamplord_attack_hello,			// 누워서 드릴처럼 돌면서 돌진

		vamplord_attack_summon_begin,	// 메테오랑 비슷한데 끝 동작이 없음
		vamplord_attack_summon_during,

		vamplord_attack_turnleft2_hard,
		vamplord_attack_turrightt2_hard,

		vamplord_attack02_begin_hard,	//
		vamplord_attack02_end_hard,
		vamplord_attack02_hard,			// 한손치기

		vamplord_attack03_fast_hard,	// 넓게 움직이면서 5타
		vamplord_attack03_hard,			// 제자리에서 5타

		vamplord_laugh,

		vamplord_swamp_begin,	// 피웅덩이 장판 공격
		vamplord_swamp_during,
		vamplord_swamp_end,
		vamplord_swamp_ready,

		vamplord_threat,

		vamplord_wrap_air_hard,		
		vamplord_wrap_air_hard_end,	// 블링크 후

		vamplord_wrap_air2_begin,
		vamplord_wrap_air2_during,
		vamplord_wrap_air2_end,		// 블링크 후 공격

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
		duel_lose_damage,	//쳐내기 당했을때 모션
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
	DDT_FRONT,		//전방
	DDT_BACK,		//후방
	DDT_FRONT_LEFT,		//전방 좌측
	DDT_BACK_LEFT,		//후방 좌측
	DDT_FRONT_RIGHT,	//전방 우측
	DDT_BACK_RIGHT,		//후방 우측
};

enum DFONT_FONT_TYPE	// 폰트의 종류
{ 
	DFONT_TYPE_DAMAGE,		// 데미지 폰트를 사용할때 
	DFONT_TYPE_OVERKILL,	// 오버킬 폰트를 사용할때
	DFONT_TYPE_END
};	// 폰트의 종류를 나눔

enum DFONT_OBJ_TYPE			// 데미지를 받는 객체
{ 
	DFONT_OBJ_NULL,			// 초기값 상태 / 오버킬일때 / 색깔을 안넣고 싶을때
	DFONT_OBJ_PLAYER,		// 플레이어가 피격당할때
	DFONT_OBJ_MONSTER ,		// 몬스터가 피격당할때
	DFONT_STATE_END
};	// 데미지 폰트의 상태를 나눔

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

		//공용 이펙트
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

