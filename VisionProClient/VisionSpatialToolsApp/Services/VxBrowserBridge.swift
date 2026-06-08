import Foundation
import Combine

// MARK: - VxTab (tab.rsのSwift移植)
// verantyx-browserのTab構造体を忠実に再現。
// 通常タブ / プライベートタブ / 履歴 / タイトル管理。

struct VxTab: Identifiable {
    let id: UUID
    var url: String
    var title: String
    var isPrivate: Bool
    var history: [String]
    var isLoading: Bool

    init(url: String = "", isPrivate: Bool = false) {
        self.id        = UUID()
        self.url       = url
        self.title     = isPrivate ? "🔒 Private" : "New Tab"
        self.isPrivate = isPrivate
        self.history   = []
        self.isLoading = false
    }

    /// tab.rsのsummary()相当 (タブバー表示用)
    var summary: String {
        let mark = isPrivate ? "🔒" : ""
        let t = title.isEmpty ? "(empty)" : title
        let truncated = t.count > 22 ? String(t.prefix(20)) + "…" : t
        return "\(mark)\(truncated)"
    }

    mutating func pushHistory(_ url: String) {
        history.append(url)
        self.url = url
    }

    mutating func goBack() -> String? {
        guard history.count >= 2 else { return nil }
        history.removeLast()
        let prev = history.last ?? url
        self.url = prev
        return prev
    }
}

// MARK: - VxTabManager (TabManager.rsのSwift移植)
// マルチタブ管理。active tab切り替え、作成、クローズ。

@MainActor
class VxTabManager: ObservableObject {

    @Published var tabs: [VxTab] = []
    @Published var activeTabID: UUID?

    init() {
        let initial = VxTab(url: "https://www.google.com")
        tabs.append(initial)
        activeTabID = initial.id
    }

    var activeTab: VxTab? {
        tabs.first { $0.id == activeTabID }
    }

    var activeIndex: Int? {
        tabs.firstIndex { $0.id == activeTabID }
    }

    func newTab(isPrivate: Bool = false) {
        let tab = VxTab(isPrivate: isPrivate)
        tabs.append(tab)
        activeTabID = tab.id
    }

    func closeTab(id: UUID) {
        guard tabs.count > 1 else { return }  // 最後のタブは閉じない
        if let idx = tabs.firstIndex(where: { $0.id == id }) {
            tabs.remove(at: idx)
            // アクティブが閉じられた場合は隣に移動
            if activeTabID == id {
                let newIdx = min(idx, tabs.count - 1)
                activeTabID = tabs[newIdx].id
            }
        }
    }

    func switchTo(id: UUID) {
        if tabs.contains(where: { $0.id == id }) {
            activeTabID = id
        }
    }

    func updateActiveTab(title: String? = nil, url: String? = nil, isLoading: Bool? = nil) {
        guard let idx = activeIndex else { return }
        if let t = title     { tabs[idx].title     = t }
        if let u = url       { tabs[idx].url       = u }
        if let l = isLoading { tabs[idx].isLoading = l }
    }

    func navigateActive(to url: String) {
        guard let idx = activeIndex else { return }
        tabs[idx].pushHistory(url)
        tabs[idx].isLoading = true
    }

    func goBack() -> String? {
        guard let idx = activeIndex else { return nil }
        return tabs[idx].goBack()
    }
}

// MARK: - VxBridgeCommand (bridge.rsのCommand enumのSwift版)
// stealth_bridge.rsのBridgeCommandをSwiftに移植。

struct VxBridgeCommand {
    enum Kind {
        case navigate(url: String)
        case goBack
        case goForward
        case reload
        case evalJS(script: String)
        case injectKey(key: String)
        case injectKeyToPage(key: String)
        case getPage
        case ping
    }
    let kind: Kind
    let id: UUID
    init(_ kind: Kind) {
        self.kind = kind
        self.id   = UUID()
    }
}

// MARK: - VxBridgeResponse (bridge.rsのBridgeResponseのSwift版)
struct VxBridgeResponse {
    enum Status { case ok, error, navigating, hitlDone, evalOk, evalError, pong }
    let id:       UUID?
    let status:   Status
    let message:  String?
    let url:      String?
    let title:    String?
    let markdown: String?
}
