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


// VertexShading�� �Է°����� ���� ������ ����
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


// VertexShading : ������ ��ȯ (������ ��ȯ)
VS_OUT/*����*/ VS_MAIN(VS_IN In/*����*/)
{
	VS_OUT		Out = (VS_OUT)0;

	// matWV : ���� * ��
	// matWVP : ���� * �� * ����
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
// technique : ��ġ�� �������ο����� �ٸ� ���̵��� �� �� �ֵ��� �Ѵ�.
technique Default
{
	// pass : ����� ĸ��ȭ
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