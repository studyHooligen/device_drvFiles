# AUDIO

- 音频标准
- 音频文件类型
- 音频文件重要参数
- 音频接口

## 音频标准

- RIFF

## 音频文件类型

- WAVE

### WAVE

**NOTE**:无特殊标注位数，默认32位

- RIFF WAVE Chunk
  - chunnkID：固定"RIFF"
  - ChunkSize：文件总大小-8
  - format：文件格式"WAVE"
- Format Chunk
  - chunkID：固定"fmt"
  - chunkSize：自集合大小，20
  - 16'AudioFormat：音频格式
    - 0x10表示线性
    - 0x11表示IMA ADPCM
  - 16'NumOfChannels：通道数量
  - SampleRate：采样率
    - 0x1f40表示8KHz
  - ByteRate
  - 16'BlockAlign
  - 16'BitsPerSample
- Fact Chunk
  - chunkID：固定"FACT"
  - chunkSize：子集大小，4
  - DataFactSize：数据转PCM后大小
- Data Chunk
  - chunkID：固定"data"
  - chunkSize
  - data

## 音频文件重要参数

- 编码格式
  - PCM（脉冲编码调制）
- 采样频率
- 声道数量
- 数据精度/位数

## 音频接口

- I2S
