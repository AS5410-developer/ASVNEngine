struct VertexOutput
{
    float3 color;
    float4 position : SV_Position;
};
struct VertexInput
{
    float3 position;
    float3 color;
    float2 texCoords;
};

struct PushConst
{
    float4x4 matrix;
};

[[vk::push_constant]]
PushConst data;

[shader("vertex")]
VertexOutput VertexMain(VertexInput input)
{
    VertexOutput output;
    output.position = mul(data.matrix, float4(input.position, 1.0));
    output.color = input.color;

    return output;
}

[shader("fragment")]
float4 FragmentMain(VertexOutput inVert) : SV_Target
{
    float3 color = inVert.color;
    return float4(color, 1.0);
}
