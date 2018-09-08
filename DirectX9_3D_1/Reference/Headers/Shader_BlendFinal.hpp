
matrix		g_matWVP;

texture		g_HDRTexture;
sampler HDRSampler = sampler_state{
	texture = g_HDRTexture;
};
texture		g_EffectTexture;
sampler g_EffectSampler = sampler_state{
	texture = g_EffectTexture;
};


float2		Center = { 0.5, 0.5 };
float		BlurStart = 1.f;
float		BlurWidth = -0.1f;
int			ample = 10;

float		g_fMinus = 0.2f;

float		g_fWidthTexel;
float		g_fHeightTexel;


struct VS_IN
{
	float4	vPos	: POSITION;
	float2	vTexUV	: TEXCOORD0;

};
struct VS_OUT
{
	float4		vPos	: POSITION;
	float2		vTexUV	: TEXCOORD0;
	//float4		vProjPos	: TEXCOORD1;
};

VS_OUT VS_MAIN(VS_IN In) 
{
	VS_OUT			Out = (VS_OUT)0;

	Out.vPos			= In.vPos;
	Out.vTexUV.x		= In.vTexUV.x + g_fWidthTexel;
	Out.vTexUV.y		= In.vTexUV.y + g_fHeightTexel;

	return Out;
}
struct PS_IN
{
	float2		vTexUV	: TEXCOORD0;
};
struct PS_OUT
{
	vector	vColor : COLOR0;
};

PS_OUT PS_MAIN( PS_IN In )
{
	PS_OUT			Out = (PS_OUT)0;

	float dist = distance( In.vTexUV, float2(0.5,0.5)) * 1.5;    

	vector		vResult[3];
	//vResult[2] = saturate(tex2D(g_NoHDREffectSampler,  In.vTexUV) - vector(g_fMinus, g_fMinus, g_fMinus, 0.f));
	vResult[1] = tex2D(HDRSampler,  In.vTexUV) + tex2D(g_EffectSampler,  In.vTexUV) /*+ vResult[2]*/;

	float fDivide = 1 / (float)(ample -1);
	In.vTexUV -= Center;

	for(int i = 0; i < ample; ++i)
	{
		float soale = BlurStart + BlurWidth * (i * fDivide);
		vResult[0] += tex2D(HDRSampler, In.vTexUV * soale + Center) + tex2D(g_EffectSampler, In.vTexUV * soale + Center) /*+ saturate(tex2D(g_NoHDREffectSampler,  In.vTexUV * soale + Center) - vector(g_fMinus, g_fMinus, g_fMinus, 0.f))*/;
	}
	vResult[0] *= fDivide;

	float fLinear = smoothstep(0, 1, dist);
	Out.vColor = (1 - fLinear) * vResult[1] + fLinear * vResult[0];

	return Out;
}
// pass 2 /////////////////////////////////////////////////////////////////////////////////////////////////////////////
VS_OUT VS_NoRadial(VS_IN In) 
{
	VS_OUT			Out = (VS_OUT)0;

	Out.vPos			= In.vPos;
	Out.vTexUV.x		= In.vTexUV.x + g_fWidthTexel;
	Out.vTexUV.y		= In.vTexUV.y + g_fHeightTexel;

	return Out;
}
PS_OUT PS_NoRadial( PS_IN In )
{
	PS_OUT			Out = (PS_OUT)0;

	vector		vResult[3];
	vResult[0] = tex2D(HDRSampler,  In.vTexUV);
	vResult[1] = tex2D(g_EffectSampler,  In.vTexUV);
	//vResult[2] = saturate(tex2D(g_NoHDREffectSampler,  In.vTexUV) - vector(g_fMinus, g_fMinus, g_fMinus, 0.f));

	Out.vColor = saturate(vResult[0] + vResult[1] /*+ vResult[2]*/);
	
	return Out;
}

technique Default
{
	// pass : ±â´ÉÀÇ Ä¸½¶È­
	pass Blend_Final
	{		
		VertexShader	= compile vs_3_0 VS_MAIN();
		PixelShader		= compile ps_3_0 PS_MAIN();
	}
	pass Blend_FinalNoRadial
	{		
		VertexShader	= compile vs_3_0 VS_NoRadial();
		PixelShader		= compile ps_3_0 PS_NoRadial();
	}
};