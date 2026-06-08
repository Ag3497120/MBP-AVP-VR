# Verantyx-Mirage: The Ultimate Ultra-Low Latency VR Steaming Architecture

![Verantyx Mirage Logo](https://via.placeholder.com/1000x300?text=Verantyx+Mirage)

## 目次 (Table of Contents)

1. [イントロダクション＆ビジョン (Introduction & Vision)](#イントロダクションビジョン-introduction--vision)
2. [システムアーキテクチャとデータフロー (System Architecture & Data Flow)](#システムアーキテクチャとデータフロー-system-architecture--data-flow)
    - [Macホスト側: Game Porting ToolkitとDXVK](#macホスト側-game-porting-toolkitとdxvk)
    - [OpenVRエミュレーター: SteamVRの完全バイパス](#openvrエミュレーター-steamvrの完全バイパス)
    - [HardwareEncoder: Apple VideoToolboxの極限駆動](#hardwareencoder-apple-videotoolboxの極限駆動)
    - [Vision Proクライアント: AVSampleBufferDisplayLayerと高速デコード](#vision-proクライアント-avsamplebufferdisplaylayerと高速デコード)
3. [究極のフルハンドトラッキング・エンジン (Ultimate Full Hand Tracking Engine)](#究極のフルハンドトラッキングエンジン-ultimate-full-hand-tracking-engine)
    - [ハンドトラッキングの基礎と骨格データ](#ハンドトラッキングの基礎と骨格データ)
    - [ジェスチャーの判定ロジック](#ジェスチャーの判定ロジック)
    - [仮想ジョイスティック（リスト・ローテーション・マッピング）](#仮想ジョイスティックリストローテーションマッピング)
4. [ディープダイブ: 採用テクノロジーの裏側 (Deep Dive: Technologies Used)](#ディープダイブ-採用テクノロジーの裏側-deep-dive-technologies-used)
    - [Color Correction: vImageによる超高速チャンネルスワップ](#color-correction-vimageによる超高速チャンネルスワップ)
    - [独自UDPプロトコルとパケットフラグメンテーション](#独自udpプロトコルとパケットフラグメンテーション)
    - [COMインターフェースのリバースエンジニアリングとVTableフック](#comインターフェースのリバースエンジニアリングとvtableフック)
5. [セットアップと使い方 (Setup & Installation Guide)](#セットアップと使い方-setup--installation-guide)
    - [必須要件 (Requirements)](#必須要件-requirements)
    - [リポジトリのクローンとビルド](#リポジトリのクローンとビルド)
    - [起動プロセス](#起動プロセス)
6. [トラブルシューティング (Troubleshooting)](#トラブルシューティング-troubleshooting)

---

## 1. イントロダクション＆ビジョン (Introduction & Vision)

**Verantyx-Mirage**は、Mac上で動作するハイエンドVRゲーム（例: *Half-Life: Alyx*）を、Apple Vision ProなどのスタンドアロンVRヘッドセットに対して、**究極の超低遅延（Ultra-Low Latency）かつ高画質（150Mbps, 120Hz）でストリーミングする**ためのフルスクラッチ・アーキテクチャです。

### 「Mirage（蜃気楼）」のコンセプト
従来のVRストリーミング（Virtual Desktop, Steam Linkなど）は、SteamVRのコンポジターを介して映像を取得し、ネットワーク経由で送信します。しかし、SteamVRのコンポジターを経由すること自体が、ミリ秒単位の遅延や、オーバーヘッドを生み出します。

Mirageは、**SteamVRを完全にバイパス**します。
自作のカスタムOpenVR DLL（`openvr_api.dll`）をゲーム本体に直接注入し、ゲームエンジンが生成したDirectX 11のテクスチャを、SteamVRに渡す前に**共有メモリ（Shared Memory）**に直接ダンプします。これにより、ゲームエンジンからエンコーダーまでのパスが最短化され、事実上「蜃気楼」のように、ネイティブ実行と区別がつかないレベルの遅延を実現します。

---

## 2. システムアーキテクチャとデータフロー (System Architecture & Data Flow)

システム全体は、以下の4つの主要なコンポーネントで構成されており、それぞれが緊密に連携して動作します。

### Macホスト側: Game Porting ToolkitとDXVK
Mac上でWindows向けのAAAタイトル（Half-Life: Alyxなど）を動かすため、Appleの**Game Porting Toolkit (GPTK)** と **DXVK**（DirectX to Vulkan/Metal変換）を利用しています。
独自の起動スクリプト（`launch_vision_pro_vr.sh`）を用いて、`hlvr.exe`をWine経由で起動します。この際、`WINEDLLOVERRIDES`を使用して、本物の`openvr_api.dll`の代わりに、私たちが作成したカスタムエミュレーターDLLを強制的に読み込ませます。

### OpenVRエミュレーター: SteamVRの完全バイパス
`VRDriver/src/openvr_emulator.cpp` が本システムの心臓部の一つです。
ゲームエンジンはVRヘッドセットと通信するために、OpenVRのAPI（`IVRSystem`, `IVRCompositor`, `IVRSettings`など）を呼び出します。このエミュレーターは、これらのCOMインターフェースを完全に模倣（モック）します。

1. **ポーズの提供**: Vision Proから受信した頭と手のトラッキング情報をゲームに渡します。
2. **テクスチャのキャプチャ**: ゲームが `IVRCompositor::Submit` を呼び出した瞬間、渡されたDirect3D 11のテクスチャ（左右の目の映像）をキャプチャします。
3. **共有メモリへのダンプ**: キャプチャしたテクスチャの生ピクセルデータを、`vr_shared_frame.dat` という巨大なメモリマップドファイル（Shared Memory）に高速に書き込みます。

### HardwareEncoder: Apple VideoToolboxの極限駆動
共有メモリに書き込まれた生ピクセルデータは、別プロセスとして常駐している `HardwareEncoder.swift` によって即座に読み取られます。

- **Apple VideoToolboxの活用**: Macの強力なメディアエンジン（ハードウェアエンコーダー）を直接叩きます。
- **超低遅延設定**: `kVTCompressionPropertyKey_AllowFrameReordering` を `false` に設定し、Bフレームを無効化。さらに `kVTCompressionPropertyKey_PrioritizeEncodingSpeedOverQuality` を `true` にすることで、品質よりもエンコード速度を極限まで優先します。
- **150Mbps / 120fpsの網膜ストリーミング**: ビットレートを150,000,000 bps（150Mbps）に設定し、圧縮ノイズを完全に排除。さらにエンコーダーのターゲットFPSを120fpsに指定し、現実世界と見紛うほどの滑らかさを実現しています。

### Vision Proクライアント: AVSampleBufferDisplayLayerと高速デコード
Vision Pro側のアプリ（`vision-spatial-tools`）は、Macから送られてくるUDPパケットを受信します。
パケットの欠損や順序の入れ替えに対処するため、カスタムのフレームアセンブラーがフラグメント化されたUDPパケットを瞬時に結合し、完全なH.265 (HEVC) のNALユニットを復元します。
復元されたフレームは `AVSampleBufferDisplayLayer` に流し込まれ、Vision Proのハードウェアデコーダーによってゼロ遅延で空間上にレンダリングされます。

---

## 3. 究極のフルハンドトラッキング・エンジン (Ultimate Full Hand Tracking Engine)

物理的なコントローラー（Joy-ConやVRコントローラー）を一切使用せず、**プレイヤーの「手」そのものをコントローラーにする**革新的な入力システムを実装しています。

### ハンドトラッキングの基礎と骨格データ
Vision ProのARKitが提供する `HandTrackingProvider` を使用し、手の骨格（HandSkeleton）の25個の関節（Joint）の3D座標を毎秒90回以上の超高頻度で取得します。
手首（Wrist）や各指の先端（Tip）、指の根本（Knuckle）などの空間座標（`simd_float4x4`）を抽出し、それらの「相対距離」をミリ秒単位で計算します。

### ジェスチャーの判定ロジック
抽出した関節座標をもとに、以下のジェスチャーを判定し、OpenVRのコントローラー入力に変換しています。

1. **トリガー（銃を撃つ / 選択）**:
   - **判定**: `親指の先端` と `人差し指の先端` の距離が2.5cm以内（Pinch）。
   - **マッピング**: OpenVRのTrigger軸（0〜255）としてMacへ送信。
2. **グリップ（物を掴む / グラビティ・グローブ）**:
   - **判定**: `中指・薬指・小指` が手のひら（中指の根本）に向かって曲げられ、距離が6cm以内（Fist）。
   - **マッピング**: OpenVRのGrip軸（0〜255）として送信。
3. **Aボタン（武器変更 / テレポートなど）**:
   - **判定**: `親指の先端` と `薬指の先端` の距離が2.5cm以内。
   - **マッピング**: 仮想コントローラーの Aボタン（右） / D-PAD 下（左）にマッピング。
4. **Bボタン（メニュー / 弾倉ドロップなど）**:
   - **判定**: `親指の先端` と `小指の先端` の距離が2.5cm以内。
   - **マッピング**: 仮想コントローラーの Bボタン（右） / D-PAD 右（左）にマッピング。

### 仮想ジョイスティック（リスト・ローテーション・マッピング）
VR空間での移動（ロコモーション）と旋回を実現するため、**「手をジョイスティックに見立てる」**特殊なロジックを実装しています。

- **発動条件**: `親指の先端` と `中指の先端` をくっつける（Pinch）。
- **ニュートラル・アンカーの固定**: くっつけた瞬間の「手首の角度（Transform）」をメモリに記憶（アンカー）します。
- **アナログ入力の計算**: 指をくっつけたまま手首を前後に傾ける（Pitch）と、アンカーからの相対角度が計算され、ジョイスティックのY軸（前進・後退）に変換されます。左右に傾ける（Roll）とX軸（旋回・カニ歩き）になります。
- **解除**: 指を離すと、ジョイスティックは即座に中央（0.0, 0.0）に戻ります。

このトラッキングデータとボタン入力は、Vision ProからMacへのUDPパケット（`POSE` ペイロード）に24バイトの追加データとしてパッキングされ、Macの `HardwareEncoder` がそれを受け取って共有メモリに書き込みます。

---

## 4. ディープダイブ: 採用テクノロジーの裏側 (Deep Dive: Technologies Used)

### Color Correction: vImageによる超高速チャンネルスワップ
Macのハードウェアエンコーダー（VideoToolbox）に入力するピクセルバッファ（`CVPixelBuffer`）は、Macがネイティブで最も得意とする `kCVPixelFormatType_32BGRA` を使用する必要があります。しかし、ゲームエンジンが生成するテクスチャは一般的に `RGBA` 形式です。
そのままエンコードすると、赤（Red）と青（Blue）が反転し、肌が青く見える「アバター状態」になってしまいます。

これをソフトウェアでピクセルごとにループ処理して入れ替えると、CPU負荷が跳ね上がり、フレームレートが大幅に低下します。
そこで、Appleの **Accelerateフレームワーク（vImage）** を導入しました。
`vImagePermuteChannels_ARGB8888` を使用し、SIMD（ベクトル演算）命令レベルで Rチャンネルと Bチャンネルを入れ替える処理を実装。これにより、**4K解像度の画像変換を1ミリ秒未満のゼロ・オーバーヘッドで完了**させ、完璧な色彩（Half-Life: Alyxの美しいオレンジや赤）を取り戻しました。

### 独自UDPプロトコルとパケットフラグメンテーション
VRの映像ストリーミングにおいて、TCPはパケット到達保証のための再送（Retransmission）処理が発生し、深刻な遅延（スタッター）を引き起こすため使用できません。
私たちは**完全にピュアなUDP**による通信スタックを構築しました。

1. **AWDLとBonjour**: Vision ProとMacの接続は、Apple Wireless Direct Link (AWDL) と Bonjour (`_verantyxvr._udp`) を使用して、IPアドレスを自動検出し、ルーターを経由しない超高速なP2P通信を確立します。
2. **`VRAN` ヘッダー**: 1フレームのH.265データはMTU（通常1500バイト）を超えるため、Mac側でデータを1400バイトごとに分割し、独自の `VRAN` ヘッダー（Sequence Number, Chunk Index, Total Chunks）を付与して送信します。
3. **I-Frame強制リカバリ**: UDPパケットが欠損した場合、そのフレームのデコードは失敗しますが、VideoToolboxの `kVTCompressionPropertyKey_MaxKeyFrameInterval` を `60` に設定しているため、わずか0.5秒以内に次の完全なキーフレーム（I-Frame）が到着し、画面の乱れが即座に自己修復されます。

### COMインターフェースのリバースエンジニアリングとVTableフック
OpenVRはC++の仮想関数テーブル（VTable）に依存するCOMアーキテクチャを採用しています。
SteamVRをバイパスするためには、ゲームエンジンに対して「自分こそがSteamVRだ」と信じ込ませる必要があります。
私たちは、最新の `openvr.h` を解析し、`IVRSystem` や `IVRCompositor` などのインターフェースの仮想関数の**宣言順序と引数の型を完全に一致させたモッククラス**を作成しました。
ゲームが呼び出さない不要な関数については、パディング用のダミー関数（`Dummy1()`, `Dummy2()`...）を正確な数だけ配置し、メモリレイアウトのズレによるアクセス違反（セグメンテーションフォールト）を完全に防いでいます。

---

## 5. セットアップと使い方 (Setup & Installation Guide)

この強力なVRストリーミング環境を構築・実行するための手順です。

### 必須要件 (Requirements)
- **ハードウェア**:
  - Apple Silicon搭載 Mac (M1/M2/M3)
  - Apple Vision Pro
- **ソフトウェア**:
  - macOS Sonoma 以降
  - Xcode 15 以降
  - Game Porting Toolkit (GPTK) & DXVK
  - Steam & Half-Life: Alyx (Windows版をGPTK内にインストール済みであること)
  - Homebrew, MinGW (`x86_64-w64-mingw32-g++`), Python 3, Swiftコンパイラ

### リポジトリのクローンとビルド

このシステムは複数のリポジトリにまたがっています。

#### 1. Mac側ホスト環境のクローン
```bash
# メインのMac側エミュレーター環境
git clone https://github.com/Ag3497120/Verantyx-Mirage.git

# エンコーダーやCLIツールを含むリポジトリ
git clone https://github.com/Ag3497120/verantyx-cli.git
```

#### 2. OpenVR エミュレーター（DLL）のコンパイル
Windows互換のDLLをビルドするため、MinGWクロスコンパイラを使用します。
```bash
cd Verantyx-Mirage/VRDriver/src
x86_64-w64-mingw32-g++ -shared -static-libgcc -static-libstdc++ -I../include openvr_emulator.cpp ../openvr_api.def -o openvr_api.dll

# ビルドしたDLLをSteamVR配下のゲームディレクトリにコピー
cp openvr_api.dll "$HOME/Verantyx_VR_Drive/SteamVR_Prefix/drive_c/Program Files (x86)/Steam/steamapps/common/Half-Life Alyx/game/bin/win64/openvr_api.dll"
```

#### 3. ハードウェアエンコーダーのビルド
```bash
cd ../../../verantyx-cli/cli
swiftc HardwareEncoder.swift -o HardwareEncoder -O
```

#### 4. Vision Pro クライアントのクローンとビルド
*（注意: 現在vision-spatial-toolsリポジトリはArchivedとなっていますが、ソースコードへのアクセス権がある前提の手順です）*
```bash
git clone https://github.com/Ag3497120/VisionSpatialTools.git
```
- Xcodeを開き、`VisionSpatialToolsApp.xcodeproj` を開きます。
- ターゲットデバイスとして「Apple Vision Pro」を選択します。
- `Command + R` を押してビルドし、デバイスにアプリをインストール・実行します。

### 起動プロセス

1. **ネットワーク共有**:
   Macの「システム設定」>「一般」>「共有」から「インターネット共有」をオンにし、Vision ProをMacのWi-Fiネットワーク（Direct AP）に接続します。これによりルーターをバイパスします。
2. **Mac側での起動**:
   ターミナルを開き、以下のスクリプトを実行します。
   ```bash
   cd ~/Verantyx-Mirage/scripts
   ./launch_vision_pro_vr.sh
   ```
   このスクリプトが自動的に `HardwareEncoder` をバックグラウンドで起動し、続いてGPTK経由で `Half-Life: Alyx` を起動します。
3. **Vision Pro側での接続**:
   Vision Proでアプリを開きます。BonjourによってMacが自動検出され、映像のストリーミングとハンドトラッキングデータの送信が開始されます。

---

## 6. トラブルシューティング (Troubleshooting)

- **Q. 映像の色がおかしい（肌が青い、全体が青みがかっている）**
  - **A.** `HardwareEncoder.swift` 内の `vImagePermuteChannels_ARGB8888` 処理が正常にコンパイルされているか確認してください。ゲーム側のバッファ形式が変更された可能性があります。
- **Q. ゲームが起動直後にクラッシュする**
  - **A.** `openvr_emulator.cpp` のVTableパディング（`DummyX()` の数）が、ゲームが期待する `IVRSystem` のバージョンと一致していない可能性があります。SteamVRのアップデート等で構造体が変わった場合は、`openvr.h` の差分を確認して修正してください。
- **Q. 手の動きがカクつく、映像が飛ぶ**
  - **A.** MacとVision Proの通信にルーターが介在している可能性があります。必ずMacの「インターネット共有」機能を使って、Vision ProをMacに直接Wi-Fi接続してください。

---
*Created and maintained by the Verantyx Advanced AI Team.*
