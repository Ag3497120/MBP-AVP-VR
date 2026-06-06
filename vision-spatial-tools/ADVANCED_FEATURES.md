# Advanced Features - 高度な機能ガイド

Vision Spatial Toolsの高度な機能について詳しく説明します。

## 目次
1. [トラックパッド](#トラックパッド)
2. [オブジェクト認識とスキャン](#オブジェクト認識とスキャン)
3. [磁気吸着システム](#磁気吸着システム)
4. [オブジェクト追従](#オブジェクト追従)
5. [アダプティブサイジング](#アダプティブサイジング)

---

## トラックパッド

### 概要
MacBookのトラックパッドのような、マルチタッチ対応の仮想トラックパッドです。

### 機能

#### 基本操作
- **シングルタッチ**: カーソル移動
- **タップ**: クリック
- **ドラッグ**: 選択とドラッグ

#### マルチタッチジェスチャー
- **2本指スクロール**: コンテンツのスクロール
- **2本指ピンチ**: ズームイン/アウト
- **3本指スワイプ**: アプリ切り替え、Exposé

#### ビジュアルフィードバック
- リアルタイムカーソル表示
- タップ時の視覚的フィードバック
- エッジジェスチャーのガイド表示

### 技術仕様

```swift
// トラックパッドサイズ
幅: 15cm
高さ: 10cm
厚さ: 1cm

// タッチ検出精度
解像度: 2D座標 (-1 to 1)
サンプリングレート: 60Hz
マルチタッチ: 最大10点同時

// マテリアル
ベース: メタリックガラス風 (透明度95%)
表面: 低摩擦素材 (roughness: 0.1)
```

### 使用例

```swift
// トラックパッドの作成
let trackpad = TrackpadEntity()
trackpad.position = [0, 0.1, 0]

// タッチイベントの処理
trackpad.handleTouch(at: localPosition)
trackpad.handleTouchMove(at: newPosition)
trackpad.handleTouchEnd()

// ジェスチャーの取得
let touchPosition = trackpad.currentTouchPosition
let scrollVelocity = trackpad.currentScrollVelocity
```

---

## オブジェクト認識とスキャン

### 概要
現実世界のオブジェクトを3Dスキャンし、その形状、サイズ、位置を認識します。

### スキャンプロセス

#### 1. スキャンモードの有効化
```
メインUI → "オブジェクトスキャン" トグルをON
```

#### 2. オブジェクトのスキャン
1. スキャンしたいオブジェクトに近づく
2. オブジェクトをタップ
3. 青い球体が表示され、スキャン開始
4. 約2秒でスキャン完了
5. 緑色の境界ボックスが表示される

#### 3. スキャンデータ
```swift
struct ScannedObject {
    id: UUID                    // 一意の識別子
    position: SIMD3<Float>      // 3D位置
    boundingBox: SIMD3<Float>   // サイズ (幅, 高さ, 奥行)
    orientation: simd_quatf     // 向き
    confidence: Float           // 検出信頼度 (0-1)
    mesh: MeshResource?         // 3Dメッシュデータ
    surfaceNormal: SIMD3<Float> // 表面の法線
    isFlatSurface: Bool         // 平面かどうか
}
```

### 対応オブジェクト

| オブジェクトタイプ | 検出精度 | 推奨サイズ |
|---|---|---|
| テーブル | 高 | 60cm+ |
| ノートPC | 高 | 30-40cm |
| 本 | 中 | 15-30cm |
| マグカップ | 中 | 8-15cm |
| 壁面 | 高 | 任意 |

### 技術詳細

```swift
// ObjectRecognitionManager
class ObjectRecognitionManager {
    // スキャン開始
    func scanObject(at: SIMD3<Float>) async -> ScannedObject?

    // 検出されたオブジェクト一覧
    @Published var detectedObjects: [DetectedObject]

    // スキャン中のオブジェクト
    @Published var scanningObjects: [ScannedObject]
}
```

---

## 磁気吸着システム

### 概要
ツールをオブジェクトに近づけると、自動的に吸い付いて配置されるシステムです。

### 動作原理

#### 1. 吸着範囲
```swift
吸着距離: 15cm (snapDistance)
吸着強度: 0.8 (snapStrength)
```

#### 2. 吸着プロセス
```
ツールを移動
  ↓
15cm以内にオブジェクトを検出
  ↓
磁気引力を適用 (distance: Float)
  force = direction × (0.8 × strength) × (1 / distance)
  ↓
スムーズにオブジェクトへ移動
  ↓
最適な位置・向きに配置
  ↓
吸着完了（緑のインジケーター表示）
```

### 最適配置アルゴリズム

```swift
func calculateOptimalPlacement(
    tool: Entity,
    object: ScannedObject,
    toolType: ToolType
) -> (offset: SIMD3<Float>, orientation: simd_quatf) {

    // キーボード: 表面の上、5度傾斜
    if toolType == .keyboard {
        offset = [0, objectHeight/2 + 0.005, 0]
        orientation = tilt(5°) × objectOrientation
    }

    // トラックパッド: 表面の上、フラット
    if toolType == .trackpad {
        offset = [0, objectHeight/2 + 0.002, 0]
        orientation = objectOrientation
    }

    return (offset, orientation)
}
```

### 使用方法

#### 自動吸着
1. メインUIで「磁気吸着」をON
2. オブジェクトをスキャン（緑の境界ボックス表示）
3. ツールを選択
4. オブジェクトに近づける（15cm以内）
5. 自動的に吸着

#### 手動吸着解除
- ツールを強く引っ張る（20cm以上離す）
- ロックボタンをタップ（吸着を固定/解除）

### パラメータ調整

```swift
class MagneticAttachmentManager {
    // 吸着距離（デフォルト: 15cm）
    private let snapDistance: Float = 0.15

    // 吸着強度（デフォルト: 0.8）
    private let snapStrength: Float = 0.8

    // アニメーション速度
    private let attractionSpeed: Float = 0.1
    private let rotationSpeed: Float = 0.05
}
```

---

## オブジェクト追従

### 概要
オブジェクトを動かすと、吸着したツールが自動的に追従します。

### 追従タイプ

#### 1. 位置追従 (Position Following)
オブジェクトの移動に追従
```swift
followPosition: true  // 位置を追従
predictionTime: 50ms  // 50ms先を予測
```

#### 2. 回転追従 (Orientation Following)
オブジェクトの回転に追従
```swift
followOrientation: true  // 回転を追従
smoothingFactor: 0.1     // 回転のスムージング
```

#### 3. 反転対応 (Flip Detection)
オブジェクトを裏返しても追従
```swift
stickOnFlip: true  // 裏返し時も吸着維持

// 裏返し検出
let upVector = orientation.act([0, 1, 0])
let isFlipped = dot(upVector, [0, 1, 0]) < 0

if isFlipped {
    // ツールを裏側に移動
    offset.y = -(objectHeight/2 + 0.01)
}
```

### トラッキングシステム

```swift
class ObjectTrackingManager {
    // 更新頻度
    updateFrequency: 60Hz

    // スムージング
    positionSmoothing: 0.15  // 位置
    rotationSmoothing: 0.1   // 回転

    // 予測
    predictionTime: 50ms     // 未来予測
}
```

### 追従の仕組み

```
1. ARKitがオブジェクト位置を更新
   ↓
2. ObjectTrackingManagerが変化を検出
   velocity = (newPos - oldPos) / deltaTime
   ↓
3. 50ms先の位置を予測
   predictedPos = currentPos + velocity × 0.05
   ↓
4. ツールをスムーズに移動
   newPos = lerp(currentPos, predictedPos, 0.15)
   ↓
5. 画面に反映 (60FPS)
```

### 速度予測

```swift
// 位置予測
func predictPosition(for id: UUID) -> SIMD3<Float>? {
    let velocity = trackedObjects[id].velocity
    let predictedPosition = currentPosition + velocity × 0.05
    return predictedPosition
}

// 回転予測
func predictOrientation(for id: UUID) -> simd_quatf? {
    let angularVelocity = trackedObjects[id].angularVelocity
    let predictedOrientation = angularVelocity × currentOrientation
    return predictedOrientation
}
```

---

## アダプティブサイジング

### 概要
オブジェクトのサイズに応じて、ツールのサイズが自動調整されます。

### サイジングルール

#### キーボード
```swift
// オブジェクト幅の80%にスケール
scale = (objectWidth / 0.4) × 0.8

// 例
objectWidth = 0.3m (30cm MacBook)
  → scale = (0.3 / 0.4) × 0.8 = 0.6
  → keyboard width = 0.4m × 0.6 = 0.24m (24cm)

// 最大スケール: 150%
scale = min(scale, 1.5)
```

#### トラックパッド
```swift
// オブジェクト幅の40%にスケール
scale = (objectWidth / 0.15) × 0.4

// 例
objectWidth = 0.3m
  → scale = (0.3 / 0.15) × 0.4 = 0.8
  → trackpad width = 0.15m × 0.8 = 0.12m (12cm)

// 最大スケール: 120%
scale = min(scale, 1.2)
```

#### メモ・描画
```swift
// オブジェクトの奥行きに合わせる
scale = (objectDepth / 0.3) × 0.9

// アスペクト比を維持
scale = min(objectWidth / canvasWidth, objectDepth / canvasDepth)
```

#### 計算機
```swift
// 小さめに維持（オブジェクト幅の30%）
scale = (objectWidth / 0.25) × 0.3
```

### スケール制限

```swift
// すべてのツール共通
minimumScale: 0.3  // 30%まで縮小可能
maximumScale: 2.0  // 200%まで拡大可能

// 品質維持のため
if scale < 0.3 {
    // テクスチャ解像度を上げる
    // UIエレメントのサイズを維持
}
```

### 動的調整

```swift
class AdaptiveToolFollower {
    func updateToolSize(_ tool: FollowingTool, trackingInfo: TrackedObjectInfo) {
        let objectSize = trackingInfo.boundingBox
        let scale = calculateScale(toolType: tool.toolType, objectSize: objectSize)

        // スムーズなスケール変更
        let currentScale = tool.toolEntity.scale.x
        let newScale = lerp(currentScale, scale, t: 0.1)

        tool.toolEntity.scale = [newScale, newScale, newScale]
    }
}
```

### 例: ノートPCへの配置

```
対象: MacBook Pro 16インチ
サイズ: 幅35.5cm × 奥行24.8cm × 高さ1.6cm

1. スキャン結果
   boundingBox = [0.355, 0.016, 0.248]

2. キーボード配置
   scale = (0.355 / 0.4) × 0.8 = 0.71
   keyboard width = 0.4 × 0.71 = 28.4cm
   position = [0, 0.016/2 + 0.005, 0] = [0, 0.013, 0]
   tilt = 5度

3. トラックパッド配置
   scale = (0.355 / 0.15) × 0.4 = 0.95
   trackpad width = 0.15 × 0.95 = 14.25cm
   position = [0, 0.016/2 + 0.002, -0.1] (手前側)
```

---

## 実装例

### 完全な使用フロー

```swift
// 1. オブジェクトをスキャン
appModel.isScanningMode = true
// ユーザーがオブジェクトをタップ
let scannedObject = await scanObject(at: tapPosition)

// 2. トラッキング開始
let trackingId = objectTrackingManager.trackObject(scannedObject)

// 3. ツールを配置
let keyboard = KeyboardEntity()
let attachment = magneticAttachmentManager.attachToolToObject(
    tool: keyboard,
    toolType: .keyboard,
    scannedObject: scannedObject
)

// 4. 追従を有効化
adaptiveFollower.makeToolFollowObject(
    tool: keyboard,
    toolType: .keyboard,
    targetObjectId: scannedObject.id
)

// 5. 自動的に動作
// - オブジェクトを動かす → キーボードが追従
// - オブジェクトを裏返す → キーボードが裏側に移動
// - オブジェクトのサイズが変わる → キーボードがリサイズ
```

---

## パフォーマンス最適化

### 更新頻度

```swift
// トラッキング: 60Hz
ObjectTrackingManager: 60FPS更新

// 磁気吸着: 60Hz
MagneticAttachmentManager: 60FPS更新

// スキャン: オンデマンド
ObjectRecognitionManager: ユーザー操作時のみ
```

### メモリ管理

```swift
// 最大追従ツール数
maxFollowingTools: 10

// 最大スキャンオブジェクト数
maxScannedObjects: 20

// トラッキング信頼度の閾値
minConfidence: 0.2  // 以下で自動削除
```

### CPU/GPU負荷

- オブジェクト認識: バックグラウンドスレッド
- トラッキング: 非同期処理
- 描画: RealityKitのネイティブレンダリング

---

## トラブルシューティング

### ツールが吸着しない
- オブジェクトがスキャンされているか確認（緑の境界ボックス）
- 「磁気吸着」がONになっているか確認
- 15cm以内に近づける

### 追従が不安定
- 照明を明るくする
- オブジェクトの特徴が明確か確認
- トラッキング信頼度を確認（0.5以上が推奨）

### サイズが合わない
- `adaptiveSize = true`になっているか確認
- オブジェクトの境界ボックスが正確か確認
- スケール制限を調整（0.3-2.0）

---

## 今後の拡張

- [ ] 複数オブジェクトへの同時吸着
- [ ] カスタムサイジングルール
- [ ] ジェスチャーによる吸着解除
- [ ] オブジェクト形状への適応配置
- [ ] 機械学習による最適配置の学習

---

最終更新: 2026年3月12日
