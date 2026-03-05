
struct VertexInput {
  float2 Point;
  float2 TexCoord;
};
struct VertexOutput {
  float4 Point : SV_Position;
  float2 TexCoord;
  uint ObjectId;
};

struct Buffer {
  float4x4 Proj;
  float4x4 View;
  float4x4 Model;
  uint TextureNum;
};

[vk_binding(0, 0)]
StructuredBuffer<Buffer> Data;
[vk_binding(0, 1)]
Sampler2D Texture;

[shader("vertex")]
VertexOutput VertexMain(VertexInput in, uint instanceID: SV_InstanceID) {
  VertexOutput out;
  out.Point = mul(Data[instanceID].Proj * Data[instanceID].View * Data[instanceID].Model,
                  float4(in.Point, 0.0f, 1.0f));
  out.TexCoord = in.TexCoord;
  out.ObjectId = instanceID;
  return out;
}

[shader("fragment")]
float4 FragmentMain(VertexOutput in) {
  return Texture.Sample(in.TexCoord);
}
