matrix		g_matWVP;

texture		g_AlbedoTexture;
sampler AlbedoSampler = sampler_state{
	texture = g_AlbedoTexture;
	/*minfilter		= linear;
	magfilter		= linear;
	mipfilter		= linear;*/
};

texture		g_LightTexture;
sampler LightSampler = sampler_state{
	texture = g_LightTexture;
	//minfilter		= linear;
	//magfilter		= linear;
	//mipfilter		= linear;
};

//texture		g_DepthTexture;
//sampler DepthSampler = sampler_state{
//	texture = g_DepthTexture;
//};

texture		g_VelocityTexture;
sampler VelocitySampler = sampler_state{
	texture = g_VelocityTexture;
};

texture		g_DistortionTexture;
sampler DistortionSampler = sampler_state{
	texture = g_DistortionTexture;
};

//texture		g_EffectTexture;
//sampler EffectSampler = sampler_state{
//	texture = g_EffectTexture;
//};
//


float g_fWidthTexel;
float g_fHeightTexel;

struct VS_IN
{
	float4	vPos	: POSITION;
	float2	vTexUV	: TEXCOORD0;

};
struct VS_OUT
{
	float4	vPos	: POSITION;
	float2	vTexUV	: TEXCOORD0;
};

// pass 1 Light Blend //////////////////////////////////////////////////////////////////////////////////
VS_OUT VS_MAIN(VS_IN In)
{
	VS_OUT			Out = (VS_OUT)0;

	//Out.vPos		= mul(In.vPos, g_matWVP);
	Out.vPos			= In.vPos;
	Out.vTexUV.x		= In.vTexUV.x + g_fWidthTexel;	
	Out.vTexUV.y		= In.vTexUV.y + g_fHeightTexel;	

	return Out;
}
struct PS_OUT
{
	vector	vColor : COLOR0;
	//vector	vVelocity : COLOR1;
};

PS_OUT PS_MAIN( float2 vUV : TEXCOORD0 )
{
	PS_OUT			Out = (PS_OUT)0;

	//vector		vDepth			= tex2D(DepthSampler, vUV);
//
	float2		Trans		= vUV/* + 0.0001f*/;
	float4		Noise		= tex2D(DistortionSampler, Trans);
	float2		UV			= vUV + Noise.xy * 0.05f;		// 0.05 -> ø÷∞Ó¿« ¡§µµ

//
	vector		vColor[3];
	vColor[0]	= tex2D(AlbedoSampler, UV);
	vColor[1]	= saturate(tex2D(LightSampler, UV/* + 0.0001*/));
	//vColor[2]	= tex2D(EffectSampler, UV);

	Out.vColor = saturate(vColor[0] * vColor[1]/* + vColor[2]*/);

	return Out;
}

///// pass 2 ////////////////////////////////////////////


VS_OUT VS_Blend2(VS_IN In)
{
	VS_OUT			Out = (VS_OUT)0;

	//Out.vPos		= mul(In.vPos, g_matWVP);
	Out.vPos			= In.vPos;
	Out.vTexUV.x		= In.vTexUV.x + g_fWidthTexel;	
	Out.vTexUV.y		= In.vTexUV.y + g_fHeightTexel;	

	return Out;
}
PS_OUT PS_Blend2( float2 vUV : TEXCOORD0 )
{
	PS_OUT			Out = (PS_OUT)0;

	//const		int		SAMPLES = 20;
	//const		float	samples	= SAMPLES;

	//vector	vBlendColor		= tex2D(AlbedoSampler, vUV);
	//vector	vBackGround	= vBlendColor;
	//vector	vVeloCity		= tex2D(VelocitySampler, vUV);
	//vector	vVeloCityTex	= tex2D(VelocityTexSampler, vUV);

	//for(int i = 0; i < SAMPLES; ++i)
	//{
	//	float t = (float)(i + 1)/samples;
	//	vBlendColor += tex2D(AlbedoSampler, vVeloCityTex.xy/vVeloCity.w + t * vVeloCity.xy)/* * ((i+1)*fDivide)*/ ;
	//}
	//vBlendColor /= samples;

	//Out.vColor = /*vBackGround	 * (1 -vVeloCityTex.z) +*/ vBlendColor;
	vector		vColor[2];
	vColor[0]	= tex2D(AlbedoSampler, vUV);
	vColor[1]	= tex2D(VelocitySampler, vUV);

	Out.vColor = vColor[0] * (1 - vColor[1].a) + vColor[1];

	return Out;
}

VS_OUT VS_BlendNMB(VS_IN In)
{
	VS_OUT			Out = (VS_OUT)0;

	Out.vPos			= In.vPos;
	Out.vTexUV.x		= In.vTexUV.x + g_fWidthTexel;	
	Out.vTexUV.y		= In.vTexUV.y + g_fHeightTexel;	

	return Out;
}
PS_OUT PS_BlendNMB( float2 vUV : TEXCOORD0 )
{
	PS_OUT			Out = (PS_OUT)0;

	vector		vColor;
	vColor	= tex2D(AlbedoSampler, vUV);
	Out.vColor = vColor;
	return Out;
}

technique Default
{
	// pass : ±‚¥…¿« ƒ∏Ω∂»≠
	pass Blend
	{		
		VertexShader	= compile vs_3_0 VS_MAIN();
		PixelShader		= compile ps_3_0 PS_MAIN();
	}
	pass Blend2
	{		
		VertexShader	= compile vs_3_0 VS_Blend2();
		PixelShader		= compile ps_3_0 PS_Blend2();
	}
	pass BlendNoMotionBlur
	{		
		VertexShader	= compile vs_3_0 VS_BlendNMB();
		PixelShader		= compile ps_3_0 PS_BlendNMB();
	}
};