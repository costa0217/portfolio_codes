#include "Shader_Base.hpp"

vector				g_vLightDir; // 방향성광원의 방향벡터를 얻어오겠다.

vector				g_vLightDiffuse; // 광원의 난반사 색상을 얻어오자.
vector				g_vLightAmbient;
vector				g_vLightSpecular;

vector				g_vTerrainDiffuse; // 지형객체의 난반사 색상을 얻어온다.
vector				g_vTerrainAmbient;
vector				g_vTerrainSpecular;

vector				g_vWorldCamPos;

float				g_fTime;




struct VS_OUT
{
	vector			vPosition : POSITION;
	vector			vColor : COLOR0;
	float2			vTexUV : TEXCOORD0;
	float3			vViewDir : TEXCOORD1;
	float3			vReflection : TEXCOORD2;
	vector			vWorldPos : TEXCOORD3;
};

VS_OUT VS_MAIN(vector vPosition : POSITION
			   , vector vNormal : NORMAL
			   , float2 vTexUV : TEXCOORD0)
{
	VS_OUT			Out = (VS_OUT)0;

	Out.vPosition = mul( vector(vPosition.xyz, 1.f), g_matWorld );

	float3 viewDir = normalize(Out.vPosition.xyz - g_vWorldCamPos.xyz);
	Out.vViewDir = viewDir;

	Out.vPosition = mul( Out.vPosition, g_matView );
	Out.vPosition = mul( Out.vPosition, g_matProj );


	//Out.vPosition = vector(vPosition.xyz, 1.f);
	Out.vTexUV.x = vTexUV.x + g_fTime * 2.f ;	
	Out.vTexUV.y = vTexUV.y + g_fTime;

	vector	vWorld_Normal = normalize(mul(vector(vNormal.xyz, 0.f), g_matWorld));
	vector	vWorld_LightDirInv = normalize(g_vLightDir) * -1.f;
	vector	vWorld_Light = normalize(g_vLightDir);


	Out.vReflection = reflect(vWorld_Light, vWorld_Normal);

	Out.vColor = max(dot(vWorld_Normal, vWorld_LightDirInv), 0.f);
	Out.vWorldPos = mul(vector(vPosition.xyz, 1.f), g_matWorld);

	return Out;
}

struct PS_IN
{
	vector		vColor : COLOR0;
	float2		vTexUV : TEXCOORD0;
	float3		vViewDir : TEXCOORD1;
	float3		vReflection : TEXCOORD2;
	vector		vWorldPos : TEXCOORD3;

};
struct PS_OUT
{
	vector		vColor : COLOR0;
};


PS_OUT PS_MAIN(PS_IN In) : COLOR
{
	PS_OUT			Out = (PS_OUT)0;

	vector			vResultColor;

	float3 reflection = normalize(In.vReflection);
	float3 viewDir = normalize(In.vViewDir);
	float3 specular = 0;

	if(In.vColor.x > 0)
	{
		specular = saturate(dot(reflection, -viewDir));
		specular = pow(specular, 30.f);
	}

	vResultColor = tex2D(BaseSampler, In.vTexUV * 20.0f);	


	vector vDiffuse		= (g_vLightDiffuse * g_vTerrainDiffuse);
	vector vAmbient		= (g_vLightAmbient * g_vTerrainAmbient);
	vector vSpecular	= (g_vLightSpecular * g_vTerrainSpecular) * vector(specular.xyz, 1.f);
	vector vAllLight	= vDiffuse * (In.vColor + vAmbient) + vSpecular;

	Out.vColor = (vResultColor * vAllLight);

	return Out;
}



technique Default
{
	pass Sea
	{
		VertexShader = compile vs_3_0 VS_MAIN();
		PixelShader = compile ps_3_0 PS_MAIN();
	}
};