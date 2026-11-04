# 云霓智能眼镜 · Nimbus Glasses

> 端侧 AI + AR 光波导 + T1000 碳纤维一体机身 · 一个平台，三种配置

> **⚠️ 本项目是 NewBiz 校园商业模拟挑战赛的样本产品代码，不是可量产的真实产品。**
> 成本、毛利、时延等数字均为演示与估算口径，品牌「云霓 / Nimbus」为虚构。
> 使用、引用或二次分发前，请先阅读 [DISCLAIMER.md](DISCLAIMER.md)。

一套可复现的智能眼镜参考实现：固件、三端客户端（Android / Windows / Linux）、
第三方接入 SDK、DeepSeek V4-pro 专属适配层、量化权重配置与产测工具。
三档机型共用同一套代码，差异集中在光学模组、麦克风、结构件与电池。

**云霓 · Nimbus**：中文取自“大旱之望云霓”——久候而至的那道光；Nimbus 是光晕，
日落之后、雨云将散时留在空气里的那一圈。我们把 AR 做成这圈光：不遮挡、不打扰，
只在需要时浮现在视野边缘。代码里的 `nb_` 前缀保持不变，nb = Nimbus，与既有
ABI 一致，改名不动符号。

## 产品线

| | NB-L1 Nimbus Air | NB-S1 Nimbus | NB-P1 Nimbus Pro |
| --- | --- | --- | --- |
| 显示 | 无（纯语音） | 双目单色光波导 640×480 | 双目全彩 1280×720 |
| 麦克风 | 2 | 4 | 6 |
| 重量 | 38 g | 44 g | 48 g |
| 完全成本 | ¥2,500 | ¥4,200 | ¥6,200 |
| 建议零售价 | ¥3,499 | ¥6,999 | ¥11,999 |

产品介绍见 [docs/product-line.md](docs/product-line.md)，成本与毛利见
[docs/pricing.md](docs/pricing.md)，机型能力的唯一数据源是
[`sku/tiers.json`](sku/tiers.json)。

## 仓库结构（monorepo）

| 目录 | 作用 | 主要语言 |
| --- | --- | --- |
| `firmware/` | 端侧固件：RTOS 任务、驱动、显示与音频链路 | C / CMake |
| `app/` | Android / Windows / Linux 三端客户端 | Kotlin / C++ · Qt / Rust |
| `sdk/` | 第三方接入 SDK，一套接口跑三端 | C99 头文件 + 各语言绑定 |
| `adapter/` | DeepSeek V4-pro 专属适配层：路由、提示词、上下文压缩 | Python |
| `models/` | INT4 量化配置与 LoRA 适配卡（权重不入库） | JSON |
| `sku/` | 三档机型的 BOM、定价与能力表（唯一数据源） | JSON |
| `tools/` | 烧录、产测、成本核算脚本 | Python |
| `docs/` | 产品线、成本、架构、硬件接口、复现指南 | Markdown |

## 快速开始

```bash
# 1. 固件：编译（需要 arm-none-eabi-gcc）
cd firmware && cmake -B build -DCMAKE_TOOLCHAIN_FILE=toolchain/arm-none-eabi.cmake && cmake --build build

# 2. 适配层：本地起一个路由服务（Python 3.10+）
cd adapter && pip install -r requirements.txt && python -m src.router --port 8080

# 3. SDK 示例：编译并跑通第一个技能
cd sdk/examples/hello_skill && make && ./hello_skill
```

三端客户端请见 [app/README.md](app/README.md)；完整复现步骤见
[docs/reproduce.md](docs/reproduce.md)。固件默认按主力款（Standard）编译，
换机型加一个参数：

```bash
cmake -B build -DNB_TIER=pro   # lite | standard | pro
```

## 开发校验

改机型、改价、改 BOM 之后，跑这两条命令，别靠肉眼对账：

```bash
python tools/tier_cost.py --check              # BOM 合计 = 完全成本，价格阶梯单调
python -m unittest discover -s adapter/tests   # 机型能力表与路由降级
```

`sku/tiers.json`、`firmware/src/tier.c`、`sdk/src/nimbus_model.c` 与
`adapter/src/capabilities.py` 是同一份能力数据的四个落点，任何一处改动都要
同步其余三处，上面第二条命令会检查它们是否还一致。

## 硬件规格（以 Pro 为例）

| 项目 | 规格 |
| --- | --- |
| 整机重量 | 48 g（碳纤维一体成型机身，壁厚 1.2 mm） |
| 机身材料 | T1000 12K 单向带 + 环氧树脂，热压罐一体成型 |
| 显示 | 双目全彩光波导，1280×720，等效 3 m 外 80 英寸 |
| 音频 | 6 麦克风环形阵列 + 定向扬声器 |
| 传感器 | IMU、环境光、接近感应、深度、电容触控 |
| 续航 | 双电热插拔，单电池 8 h，全天 24 h 不间断 |
| 连接 | Wi-Fi 6E / BT 5.3 / USB-C |
| 系统 | Android、Windows、Linux 三端客户端 |
| 建议零售价 | ¥11,999 |

Air 与 Standard 的差异见 [docs/product-line.md](docs/product-line.md)。

## 开源范围与许可

除模型权重（受上游许可约束，不入库）外，本仓库全部代码以 **Apache-2.0** 发布，
可修改、可商用、可闭源分发，保留版权与许可声明即可。详见 [LICENSE](LICENSE)。

## 路线图

- **2026 · 首发**：Nimbus 与 Pro 上市，30 城线下体验
- **2027 · 下沉**：Air 上市，把入门价拉到 3,499；开发者 5,000+，技能包 200+
- **2028 · 平台**：三档共用一套技能生态，成为日常佩戴的智能入口

## 参与贡献

欢迎提交 Issue 与 PR，先读 [CONTRIBUTING.md](CONTRIBUTING.md)。
硬件接口相关问题请优先查阅 [docs/hardware-interface.md](docs/hardware-interface.md)。

## 免责声明

本仓库是 **NewBiz 校园商业模拟挑战赛的样本产品代码**，用于演示一套三档 AR 眼镜的产品
结构与工程方案，不是可量产、可销售、可佩戴上路的真实产品。

- 成本、售价、毛利、时延、续航与市场数据均为估算或设计目标，不构成采购、投资或定价依据；
- 品牌「云霓 / Nimbus」为虚构，未申请商标，与任何同名主体没有关联；
- 固件与脚本未在真实硬件上验证，请勿用于医疗、驾驶、工业控制等高风险场景；
- 代码按 “AS IS” 提供，不附带任何明示或默示担保。

完整条款（含第三方组件、安全合规、责任限制与赛事归属）见 **[DISCLAIMER.md](DISCLAIMER.md)**。
该文件不构成法律意见；真实商用前请咨询专业律师。
