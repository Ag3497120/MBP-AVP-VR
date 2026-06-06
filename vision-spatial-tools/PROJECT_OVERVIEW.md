# Vision Spatial Tools - プロジェクト概要

## プロジェクト名
**Vision Spatial Tools** - Apple Vision Pro向け空間ツールアプリケーション

## コンセプト
現実世界の任意のオブジェクト（テーブル、壁、空中など）に仮想的なツール（キーボード、メモ、描画キャンバス、計算機）を配置し、自然なジェスチャーで操作できる空間コンピューティングアプリケーション。

## 開発日
2026年3月12日

## 技術概要

### アーキテクチャ
- **パターン**: MVVM (Model-View-ViewModel)
- **UI**: SwiftUI
- **3D**: RealityKit
- **トラッキング**: ARKit (WorldTracking, PlaneDetection, HandTracking)

### コアコンポーネント

#### 1. Models
- `AppModel.swift`: アプリケーション全体の状態管理
  - ツール配置管理
  - ARKitセッション管理
  - 選択中のツールタイプ

#### 2. Views
- `ContentView.swift`: メインUI（ツール選択、設定）
- `ImmersiveView.swift`: 空間ビュー（3Dシーン、ジェスチャー処理）

#### 3. Components (3D Entities)
- `KeyboardEntity.swift`: 3Dキーボード（QWERTY配列、40キー以上）
- `MemoEntity.swift`: メモパッド（10行罫線付き）
- `DrawingCanvasEntity.swift`: 描画キャンバス（8色パレット、4段階ブラシ）
- `CalculatorEntity.swift`: 計算機（四則演算、16ボタン）

#### 4. Services
- `PersistenceManager.swift`: ツール配置の永続化
- `HandTrackingManager.swift`: ハンドジェスチャー認識
- `SpatialAnchorManager.swift`: 空間アンカー管理

## 主要機能

### ツール配置システム
1. ユーザーがツールタイプを選択
2. 空間モードに入る
3. 環境の平面を検出
4. タップでツールを配置
5. 配置情報を永続化

### ハンドトラッキング
- **ピンチジェスチャー**: 親指と人差し指が3cm以内
- **ポイントジェスチャー**: 人差し指のみ伸ばす（10cm以上）
- **グラブジェスチャー**: すべての指を握る
- リアルタイム手の位置追跡（左右独立）

### 永続化システム
- ツールタイプ、位置（4x4変換行列）、タイムスタンプを保存
- UserDefaultsにJSON形式で保存
- アプリ再起動時に自動復元

## ファイル構造

```
vision-spatial-tools/
├── Package.swift                          # Swift Package定義
├── Info.plist                             # アプリ設定（権限など）
├── README.md                              # ユーザー向けドキュメント
├── SETUP_GUIDE.md                         # セットアップ手順
├── PROJECT_OVERVIEW.md                    # このファイル
├── .gitignore                             # Git除外ファイル
└── VisionSpatialToolsApp/
    ├── VisionSpatialToolsApp.swift       # アプリエントリーポイント
    │                                      # - WindowGroupとImmersiveSpaceの定義
    │                                      # - AppModelのインジェクション
    ├── Models/
    │   └── AppModel.swift                 # 状態管理
    │                                      # - ARKitセッション
    │                                      # - ツール配置リスト
    │                                      # - 選択中のツール
    ├── Views/
    │   ├── ContentView.swift              # メインUI
    │   │                                  # - ツール選択ボタン（4種類）
    │   │                                  # - 空間モードトグル
    │   │                                  # - 状態表示
    │   └── ImmersiveView.swift            # 3D空間ビュー
    │                                      # - RealityViewの構築
    │                                      # - SpatialTapGestureの処理
    │                                      # - ツール生成とアンカリング
    ├── Components/
    │   ├── KeyboardEntity.swift           # キーボード
    │   │                                  # - 3行配列（QWERTY）
    │   │                                  # - スペースバー
    │   │                                  # - テキストディスプレイ
    │   │                                  # - ホバーエフェクト
    │   ├── MemoEntity.swift               # メモパッド
    │   │                                  # - 黄色いノート風ベース
    │   │                                  # - 10本の罫線
    │   │                                  # - クリア・保存ボタン
    │   │                                  # - テキスト管理（最大10行）
    │   ├── DrawingCanvasEntity.swift      # 描画キャンバス
    │   │                                  # - 白いキャンバス（40x30cm）
    │   │                                  # - 8色カラーパレット
    │   │                                  # - 4段階ブラシサイズ
    │   │                                  # - 描画/消去/保存機能
    │   │                                  # - ストローク管理
    │   └── CalculatorEntity.swift         # 計算機
    │                                      # - 0-9数字ボタン
    │                                      # - 四則演算ボタン
    │                                      # - C/AC/バックスペース
    │                                      # - 液晶ディスプレイ風表示
    │                                      # - 計算ロジック
    └── Services/
        ├── PersistenceManager.swift       # データ永続化
        │                                  # - SavedAnchor構造体
        │                                  # - JSON encode/decode
        │                                  # - UserDefaults保存
        │                                  # - CRUD操作
        ├── HandTrackingManager.swift      # ハンドトラッキング
        │                                  # - 左右の手の位置追跡
        │                                  # - ピンチ検出（3cm閾値）
        │                                  # - ポイント検出（10cm閾値）
        │                                  # - グラブ検出
        │                                  # - GestureType enum
        └── SpatialAnchorManager.swift     # 空間アンカー
                                           # - 平面検出
                                           # - アンカー作成
                                           # - 最近接平面検索
                                           # - 平面への投影
                                           # - アンカー移動/削除
```

## データフロー

### ツール配置フロー
```
1. User: ツール選択
   ↓
2. ContentView: selectedToolTypeを更新
   ↓
3. AppModel: 状態を保持
   ↓
4. User: 空間をタップ
   ↓
5. ImmersiveView: SpatialTapGestureを検出
   ↓
6. SpatialAnchorManager: アンカー作成
   ↓
7. ImmersiveView: ツールEntityを生成
   ↓
8. AppModel: attachedToolsに追加
   ↓
9. PersistenceManager: 永続化
```

### ジェスチャー認識フロー
```
1. ARKitSession: HandAnchor更新
   ↓
2. HandTrackingManager: アンカーを受信
   ↓
3. detectGestures(): ジェスチャー解析
   ↓
4. GestureType更新 (pinch/point/grab)
   ↓
5. Published変数経由でUIに通知
   ↓
6. ImmersiveView: ジェスチャーに応じた処理
```

### 永続化フロー
```
保存:
AppModel.attachTool()
  ↓
PersistenceManager.saveAnchor()
  ↓
JSONEncoder
  ↓
UserDefaults

復元:
PersistenceManager.init()
  ↓
UserDefaults
  ↓
JSONDecoder
  ↓
savedAnchors配列
  ↓
ImmersiveView: 各アンカーにツールを再生成
```

## 技術的な特徴

### 1. RealityKitの活用
- `Entity`ベースの3Dオブジェクト
- `ModelEntity`で形状とマテリアルを定義
- `InputTargetComponent`でインタラクション
- `HoverEffectComponent`でビジュアルフィードバック
- `CollisionComponent`で当たり判定

### 2. ARKitの統合
- `ARKitSession`で複数プロバイダーを管理
- `WorldTrackingProvider`で位置追跡
- `PlaneDetectionProvider`で平面検出
- `HandTrackingProvider`でハンドトラッキング
- リアルタイムanchorUpdatesの処理

### 3. SwiftUIとの統合
- `@StateObject`と`@Published`で状態管理
- `@EnvironmentObject`で依存性注入
- `RealityView`で3Dコンテンツ統合
- `ImmersiveSpace`で没入型体験

### 4. 空間コンピューティング
- 現実空間の理解（平面検出）
- 仮想オブジェクトの配置と固定
- 自然なジェスチャーインタラクション
- 空間的な情報の永続化

## 計算量とパフォーマンス

### メモリ使用量
- キーボード: 約40個のEntity（キー + ベース）
- メモ: 約15個のEntity（ベース + 線 + ボタン）
- 描画: 可変（ストローク数に依存、推奨最大1000線分）
- 計算機: 約20個のEntity（ボタン + ディスプレイ）

**推奨**: 同時配置ツール数 < 10個

### 処理負荷
- ハンドトラッキング: 60Hz更新
- 平面検出: 非同期、バックグラウンド
- ジェスチャー認識: O(1) - 毎フレーム定数時間
- 描画: O(n) - nはストローク数

### 最適化戦略
1. Entity数の制限
2. 不要なツールの削除
3. 描画データのバッチ処理
4. マテリアルの共有
5. LOD（Level of Detail）の実装検討

## セキュリティとプライバシー

### 権限
- **カメラ**: 環境認識に必要
- **ハンドトラッキング**: ジェスチャー操作に必要
- **ワールドセンシング**: 平面検出に必要

### データ保存
- すべてローカル（UserDefaults）
- 外部送信なし
- ユーザーのプライバシーを保護

### 安全性
- 型安全なSwift
- nil安全性（Optional）
- エラーハンドリング（do-catch）

## 拡張可能性

### 新しいツールの追加
1. 新しいEntityクラスを作成（`Components/`）
2. `ToolType` enumに追加
3. `createTool()`メソッドに分岐追加
4. UIにボタン追加

### 新しいジェスチャー
1. `HandTrackingManager.detectGestures()`を拡張
2. `GestureType` enumに追加
3. 検出ロジックを実装

### クラウド同期
1. `PersistenceManager`にCloudKit統合
2. `SavedAnchor`をCKRecordに変換
3. 同期ロジックの実装

## テスト戦略

### 単体テスト
- `AppModel`の状態遷移
- `PersistenceManager`のエンコード/デコード
- `HandTrackingManager`のジェスチャー検出ロジック
- 計算機の計算ロジック

### 統合テスト
- ツール配置フロー
- 永続化と復元
- ジェスチャーからツール操作まで

### UI/UXテスト
- 実機でのハンドトラッキング精度
- ツール配置の自然さ
- ジェスチャーの認識率

## 既知の制限

1. **シミュレーター制限**
   - ハンドトラッキング完全動作せず
   - 平面検出が限定的

2. **ツール数**
   - 多すぎるとパフォーマンス低下
   - 推奨: 10個以下

3. **描画キャンバス**
   - 大量のストロークでメモリ増加
   - 保存機能は将来実装

4. **テキスト表示**
   - 現在は簡易的な表現
   - 実際のテキストレンダリングは今後

## 今後の改善点

### 短期（v1.1）
- [ ] 実際のテキストレンダリング
- [ ] ツールのリサイズ機能
- [ ] 削除UI（ゴミ箱アイコン）
- [ ] サウンドフィードバック

### 中期（v1.5）
- [ ] より多くのツール（タイマー、ブラウザ）
- [ ] マルチユーザー対応
- [ ] 音声入力
- [ ] ツールのカスタマイズ

### 長期（v2.0）
- [ ] AIアシスタント統合
- [ ] クラウド同期
- [ ] カスタムツール作成機能
- [ ] ARコラボレーション

## ビルドとデプロイ

### 開発ビルド
```bash
xcodebuild -scheme VisionSpatialTools -destination "platform=visionOS Simulator"
```

### リリースビルド
```bash
xcodebuild -scheme VisionSpatialTools -configuration Release archive
```

### App Store提出
1. プロビジョニングプロファイル設定
2. アーカイブ作成
3. TestFlightアップロード
4. App Store Connectで審査申請

## ライセンスとクレジット

### ライセンス
MIT License

### 使用技術
- SwiftUI (Apple)
- RealityKit (Apple)
- ARKit (Apple)
- visionOS SDK (Apple)

### 開発者
Vision Spatial Tools Development Team

## バージョン情報

- **Current Version**: 1.0.0
- **Release Date**: 2026-03-12
- **Platform**: visionOS 1.0+
- **Language**: Swift 5.9

## サポートとコミュニティ

### ドキュメント
- README.md: ユーザーガイド
- SETUP_GUIDE.md: セットアップ手順
- PROJECT_OVERVIEW.md: このファイル

### 連絡先
- GitHub: (リポジトリURL)
- Email: support@vision-spatial-tools.com
- Twitter: @VisionSpatialTools

---

最終更新: 2026年3月12日
