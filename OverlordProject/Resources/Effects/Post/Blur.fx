//=============================================================================
//// Shader uses position and texture
//=============================================================================
SamplerState samPoint
{
    Filter = MIN_MAG_MIP_POINT;
    AddressU = Mirror;
    AddressV = Mirror;
};

Texture2D gTexture;

/// Create Depth Stencil State (ENABLE DEPTH WRITING)
DepthStencilState EnableDepth
{
	DepthEnable = TRUE;
};
/// Create Rasterizer State (Backface culling) 
RasterizerState BackfaceCulling
{
	CullMode = BACK;
};

//IN/OUT STRUCTS
//--------------
struct VS_INPUT
{
    float3 Position : POSITION;
	float2 TexCoord : TEXCOORD0;

};

struct PS_INPUT
{
    float4 Position : SV_POSITION;
	float2 TexCoord : TEXCOORD1;
};


//VERTEX SHADER
//-------------
PS_INPUT VS(VS_INPUT input)
{
	PS_INPUT output = (PS_INPUT)0;

	// Set the Position
    output.Position = float4(input.Position, 1);

	// Set the TexCoord
    output.TexCoord = input.TexCoord;
	
	return output;
}


//PIXEL SHADER
//------------
float4 PS(PS_INPUT input): SV_Target
{
	// Step 1: find the dimensions of the texture (the texture has a method for that)
	float2 dimension;
	gTexture.GetDimensions(dimension.x, dimension.y);

	// Step 2: calculate dx and dy (UV space for 1 pixel)	
	float2 d = 1.0f / dimension;

	// Step 3: Create a double for loop (5 iterations each)
	//		   Inside the loop, calculate the offset in each direction. Make sure not to take every pixel but move by 2 pixels each time
	//			Do a texture lookup using your previously calculated uv coordinates + the offset, and add to the final color
	float4 finalColor = (0.0f, 0.0f, 0.0f, 1.0f);
	int iterations = 5;
	for (int i = -iterations/2;  i <= iterations/2; i++)
	{
		for (int j = -iterations/2;  j <= iterations/2; j++)
		{
			// Calculate the offset using i and j
			float2 offset = float2(i * 2.0f, j * 2.0f) * d;

			// Do a texture lookup using the previously calculated uv coordinates + the offset
			float4 texColor = gTexture.Sample(samPoint, input.TexCoord + offset);

			// Add the texture color to the final color
			finalColor += texColor;
		}
	}
	
	// Step 4: Divide the final color by the number of passes (in this case 5*5)
	finalColor /= iterations * iterations;
	
	// Step 5: return the final color
	return finalColor;
}


//TECHNIQUE
//---------
technique11 Blur
{
    pass P0
    {
		// Set states...
        SetVertexShader( CompileShader( vs_4_0, VS() ) );
        SetGeometryShader( NULL );
        SetPixelShader( CompileShader( ps_4_0, PS() ) );
    }
}