matrix		g_matLightWV;
matrix		g_matLightProj;

struct VS_SHADOW_OUT
{
	float4 vPos			: POSITION;
	float4 vClipPos		: TEXCOORD1;
};


VS_SHADOW_OUT VS_SHADOW_MAIN(vector vPosition : POSITION)
{
	VS_SHADOW_OUT		Out = (VS_SHADOW_OUT)0;

	Out.vClipPos	= mul(vector(vPosition.xyz, 1.f), g_matLightWV);
	Out.vPos		= mul(Out.vClipPos, g_matLightProj);

	return Out;
}

struct PS_SHADOW_OUT
{
	float4		vLinearDepth : COLOR0;
};

PS_SHADOW_OUT PS_SHADOW_MAIN(float4 vClipPos : TEXCOORD1)
{
	PS_SHADOW_OUT		Out = (PS_SHADOW_OUT)0;

	float fDepth = vClipPos.z * 0.001f;
	Out.vLinearDepth = vector(fDepth, fDepth*fDepth, 1.f, 1.f);
	//Out.vLinearDepth = vector(1.f, 0.f, 0.f, 1.f);
	return Out;
}

//////////////////////////////////////////////////////////////

// technique : 장치의 지원여부에따라 다른 셰이딩을 할 수 있도록 한다.
technique Default
{
	// pass : 기능의 캡슐화
	pass Create_ShadowMap
	{		
		VertexShader = compile vs_3_0 VS_SHADOW_MAIN();
		PixelShader = compile ps_3_0 PS_SHADOW_MAIN();
	}
};