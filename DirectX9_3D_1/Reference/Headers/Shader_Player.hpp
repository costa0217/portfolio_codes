// �������� (Constant Table)
matrix			g_matWorld;
matrix			g_matView;
matrix			g_matProj;

texture			g_BaseTexture;

vector			g_Color;
vector			g_ColorG;
vector			g_ColorB;

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

	// matWV : ���� * ��
	// matWVP : ���� * �� * ����
	matrix		matWV, matWVP;

	matWV = mul(g_matWorld, g_matView);

	matWVP = mul(matWV, g_matProj);	

	Out.vPosition = mul(vector(In.vPosition.xyz, 1.f), matWVP);

	Out.vUV = In.vUV;

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

	float vRed;		// R ���� ��
	float vGreen;	// G ���� ��
	float vBlue;	// B ���� ��

	vRed	= ( Out.vColor.r * g_Color.r ) + ( Out.vColor.g * g_ColorG.r ) + ( Out.vColor.b * g_ColorB.r );
	vGreen	= ( Out.vColor.r * g_Color.g ) + ( Out.vColor.g * g_ColorG.g ) + ( Out.vColor.b * g_ColorB.g );
	vBlue	= ( Out.vColor.r * g_Color.b ) + ( Out.vColor.g * g_ColorG.b ) + ( Out.vColor.b * g_ColorB.b );

	Out.vColor.r = vRed;
	Out.vColor.g = vGreen;
	Out.vColor.b = vBlue;

	Out.vColor.a = 0.f;

	return Out;
}

// technique : ��ġ�� �������ο����� �ٸ� ���̵��� �� �� �ֵ��� �Ѵ�.
technique Default
{
	// pass : ����� ĸ��ȭ
	pass Render_RcTex
	{		
		VertexShader = compile vs_3_0 VS_MAIN();
		PixelShader = compile ps_3_0 PS_MAIN();
	}
};