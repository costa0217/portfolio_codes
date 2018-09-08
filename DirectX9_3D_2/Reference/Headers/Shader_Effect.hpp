// 전역변수 (Constant Table)
matrix			g_matWorld;
matrix			g_matView;
matrix			g_matProj;

texture			g_BaseTexture;
texture			g_PlusTexture;

sampler BaseSampler = sampler_state
{
	texture = g_BaseTexture;
	minfilter = linear;
	magfilter = linear;
	mipfilter = linear;
};

sampler PlusSampler = sampler_state
{
	texture = g_PlusTexture;
	minfilter = linear;
	magfilter = linear;
	mipfilter = linear;
};

// VertexShading의 입력값으로 사용될 정점의 정보
struct VS_IN
{
	vector				vPosition	: POSITION;				
	vector				vNormal	: NORMAL;
	float2				vTexUV	: TEXCOORD0;
};

struct VS_OUT
{
	vector		vPosition : POSITION;
	float2		vTexUV : TEXCOORD0;
};


// VertexShading : 정점의 변환 (영역의 변환)
VS_OUT/*정점*/ VS_MAIN(VS_IN In/*정점*/)
{
	VS_OUT		Out = (VS_OUT)0;

	// matWV : 월드 * 뷰
	// matWVP : 월드 * 뷰 * 투영
	matrix		matWV, matWVP;

	matWV = mul(g_matWorld, g_matView);
	matWVP = mul(matWV, g_matProj);	

	Out.vPosition = mul(vector(In.vPosition.xyz, 1.f), matWVP);

	Out.vTexUV = In.vTexUV;

	return Out;
}

// COLOR, TEXCOORD
struct PS_IN 
{
	float2		vUV : TEXCOORD0;
};

struct PS_OUT
{
	vector		vColor : COLOR0;
};


PS_OUT PS_MAIN(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	vector		vResult[3];
	vResult[0] = tex2D(BaseSampler, In.vUV);
	vResult[1] = tex2D(PlusSampler, In.vUV);

	/*vResult[2] = saturate(vResult[0] * vResult[1] * 3.f);*/
	clip(vResult[1] < 0.05f ? -1:1 );
	vResult[2] = vResult[0] * vResult[1];

	//(((vResultColor[0]) * (g_vLightDiffuse * g_vTerrainDiffuse)) * (vColor + (g_vLightAmbient * g_vTerrainAmbient))  + vResultColor[1]) * vResultColor[3]
	//+ vResultColor[2] * (1.f - vResultColor[3]);


	
	Out.vColor = vResult[2];
	Out.vColor.a = Out.vColor.a - 0.4f;

	return Out;
}

// technique : 장치의 지원여부에따라 다른 셰이딩을 할 수 있도록 한다.
technique Default
{
	// pass : 기능의 캡슐화
	pass Effect
	{	
		CullMode = none;

		AlphaBlendenable = true;
		Srcblend = srcalpha;
		Destblend = invsrcalpha;

		/*Zenable = false;
		Zwriteenable = false;*/

		VertexShader = compile vs_3_0 VS_MAIN();
		PixelShader = compile ps_3_0 PS_MAIN();
	}
};