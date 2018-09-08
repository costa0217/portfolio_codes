matrix			g_matWorld;
matrix			g_matView;
matrix			g_matProj;

texture			g_BaseTexture;

vector			g_vColor;

sampler BaseSampler = sampler_state
{
	texture = g_BaseTexture;
	minfilter = linear;
	magfilter = linear;
	mipfilter = linear;
};

texture			g_DepthTexture;
sampler DepthSampler = sampler_state{
	texture = g_DepthTexture;
};


// VertexShading의 입력값으로 사용될 정점의 정보
struct VS_IN
{
	vector		vPosition : POSITION;
	vector		vNormal	: NORMAL;
	float2		vUV : TEXCOORD0;
};

struct VS_OUT
{
	vector		vPosition : POSITION;
	float2			vUV : TEXCOORD0;
	vector	vClipPos : TEXCOORD1;
	vector	vWPos	: TEXCOORD2;
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

	Out.vPosition	= mul(vector(In.vPosition.xyz, 1.f), matWVP);
	Out.vClipPos	= mul(vector(In.vPosition.xyz, 1.f), matWV);

	Out.vWPos = Out.vPosition;
	Out.vUV = In.vUV;

	return Out;
}

// COLOR, TEXCOORD
struct PS_IN 
{
	float2		vUV		: TEXCOORD0;
	vector	vClipPos	: TEXCOORD1;
	vector	vWPos	: TEXCOORD2;
};

struct PS_OUT
{
	vector		vColor : COLOR0;
	vector		vDepth : COLOR1;
};


PS_OUT PS_MAIN(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	Out.vColor	= saturate(tex2D(BaseSampler, In.vUV) + g_vColor);
	Out.vDepth	=  vector(In.vClipPos.z * 0.001f, In.vClipPos.z * 0.001f, In.vClipPos.z * 0.001f, 1.f);

	float2 uv		= In.vWPos.xy / In.vWPos.w;
	uv.y			= -uv.y;
	uv				= uv * 0.5f + 0.5f;
	vector	vObjDepth = tex2D(DepthSampler, uv);

	float D = 1.f;	
	D = saturate((vObjDepth.r - (In.vClipPos.z * 0.001f)) * 1000.f);
	if(Out.vColor.a > D)
		Out.vColor.a = D;

	return Out;
}

PS_OUT PS_MAIN2(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	Out.vColor	= saturate(tex2D(BaseSampler, In.vUV) + g_vColor);
	Out.vDepth	=  vector(In.vClipPos.z * 0.001f, In.vClipPos.z * 0.001f, In.vClipPos.z * 0.001f, 1.f);

	float2 uv		= In.vWPos.xy / In.vWPos.w;
	uv.y			= -uv.y;
	uv				= uv * 0.5f + 0.5f;
	vector	vObjDepth = tex2D(DepthSampler, uv);

	float D = 1.f;	
	D = vObjDepth.r - (In.vClipPos.z * 0.001f);
	if(D > 0)
		Out.vColor.a = 1.f;
	else
		Out.vColor.a = 0.f;
		

	return Out;
}
// technique : 장치의 지원여부에따라 다른 셰이딩을 할 수 있도록 한다.
technique Default
{
	// pass : 기능의 캡슐화
	pass Render_RcTex
	{	
		CullMode					= none;
		AlphaBlendenable		= true;
		Srcblend					= srcalpha;
		Destblend				= invsrcalpha;

		VertexShader = compile vs_3_0 VS_MAIN();
		PixelShader = compile ps_3_0 PS_MAIN();
	}
	pass Render_Trail
	{	
		CullMode					= none;
		AlphaBlendenable		= true;
		Srcblend					= srcalpha;
		Destblend				= invsrcalpha;

		VertexShader	= compile vs_3_0 VS_MAIN();
		PixelShader		= compile ps_3_0 PS_MAIN2();
	}
};