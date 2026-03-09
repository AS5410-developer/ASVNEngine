
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
};

[vk_binding(0, 0)]
StructuredBuffer<Buffer> Data;
[vk_binding(1, 0)]
Sampler2D Texture;

[shader("vertex")]
VertexOutput VertexMain(VertexInput in, uint instanceID: SV_InstanceID) {
  VertexOutput out;
  Buffer data = Data[instanceID];
  out.Point = mul(mul(data.Proj, mul(data.View, data.Model)), float4(in.Point, 0.0f, 1.0f));
  out.TexCoord = in.TexCoord;
  out.ObjectId = instanceID;
  return out;
}

[shader("fragment")]
float4 FragmentMain(VertexOutput in) {
  return Texture.Sample(in.TexCoord);
}
