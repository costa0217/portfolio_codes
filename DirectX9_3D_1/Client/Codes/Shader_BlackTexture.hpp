// �������� (Constant Table)
matrix			g_matWorld;
matrix			g_matView;
matrix			g_matProj;

int				g_ICntX;
int				g_ICntY;

int				g_iFrameCnt;
int				g_iFrameHeight;
float			g_fAlpha;
vector			g_vColor;

texture			g_BaseTexture;

sampler BaseSampler = sampler_state
{
	texture = g_BaseTexture;
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
	float2		vUV : TEXCOORD0;
};


// VertexShading : ������ ��ȯ (������ ��ȯ)
VS_OUT/*����*/ VS_MAIN(VS_IN In/*����*/)
{
	VS_OUT		Out = (VS_OUT)0;

	matrix		matWV, matWVP;

	matWV = mul(g_matWorld, g_matView);

	matWVP = mul(matWV, g_matProj);	

	Out.vPosition = mul(vector(In.vPosition.xyz, 1.f), matWVP);

	Out.vUV.x = (In.vUV.x + g_iFrameCnt) / g_ICntX;
	Out.vUV.y = (In.vUV.y + g_iFrameHeight) / g_ICntY;

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

	Out.vColor = tex2D(BaseSampler, In.vUV);

	vector aa	= vector(1.f, 1.f, 1.f, 1.f);
	Out.vColor.a = 1.f  * ((Out.vColor.r + Out.vColor.g + Out.vColor.b) /** 0.3f*/);
	
	Out.vColor *= g_vColor;
	
	return Out;
}

// technique : ��ġ�� �������ο����� �ٸ� ���̵��� �� �� �ֵ��� �Ѵ�.
technique Default
{
	// pass : ����� ĸ��ȭ
	pass Render_RcTex
	{		
		CullMode = none;

		AlphaBlendenable = true;
		Srcblend = srcalpha;
		Destblend = invsrcalpha;

		VertexShader = compile vs_3_0 VS_MAIN();
		PixelShader = compile ps_3_0 PS_MAIN();
	}
};