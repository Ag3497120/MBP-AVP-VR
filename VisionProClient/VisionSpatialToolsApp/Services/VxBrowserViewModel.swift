import SwiftUI
import WebKit
import Combine

// MARK: - VxBrowserViewModel (完全版 - 人間向け機能付き)
@MainActor
class VxBrowserViewModel: ObservableObject {
    @Published var urlInput:        String  = ""
    @Published var isLoading:       Bool    = false
    @Published var isSecure:        Bool    = true
    @Published var loadProgress:    Double  = 0      // 0.0 - 1.0 プログレスバー用
    @Published var pageTitle:       String  = ""
    @Published var canGoBack:       Bool    = false
    @Published var canGoForward:    Bool    = false
    @Published var isURLBarFocused: Bool    = false
    @Published var zoomLevel:       Double  = 1.0    // 0.5 - 3.0
    @Published var findQuery:       String  = ""
    @Published var isFindBarOpen:   Bool    = false
    @Published var findResultCount: Int     = 0
    @Published var showBookmarkBar: Bool    = true
    @Published var bookmarks:       [VxBookmark] = VxBookmark.defaults
    @Published var errorMessage:    String? = nil
    @Published var isNewTabPage:    Bool    = true

    weak var webView: WKWebView?
    private var progressObserver: NSKeyValueObservation?
    private var canBackObserver:  NSKeyValueObservation?
    private var canFwdObserver:   NSKeyValueObservation?

    func attachWebView(_ wv: WKWebView) {
        webView = wv
        progressObserver = wv.observe(\.estimatedProgress) { [weak self] wv, _ in
            Task { @MainActor in self?.loadProgress = wv.estimatedProgress }
        }
        canBackObserver = wv.observe(\.canGoBack) { [weak self] wv, _ in
            Task { @MainActor in self?.canGoBack = wv.canGoBack }
        }
        canFwdObserver = wv.observe(\.canGoForward) { [weak self] wv, _ in
            Task { @MainActor in self?.canGoForward = wv.canGoForward }
        }
    }

    func navigate(tabManager: VxTabManager) {
        errorMessage = nil
        isNewTabPage = false
        var raw = urlInput.trimmingCharacters(in: .whitespaces)
        guard !raw.isEmpty else { return }
        if raw.contains(" ") || (!raw.contains(".") && !raw.hasPrefix("http")) {
            let q = raw.addingPercentEncoding(withAllowedCharacters: .urlQueryAllowed) ?? raw
            raw = "https://www.google.com/search?q=\(q)"
        } else if !raw.hasPrefix("http") {
            raw = "https://\(raw)"
        }
        urlInput = raw
        guard let url = URL(string: raw) else { return }
        tabManager.navigateActive(to: raw)
        webView?.load(URLRequest(url: url))
        isURLBarFocused = false
    }

    func goBack(tabManager: VxTabManager) {
        if webView?.canGoBack == true { webView?.goBack() }
        else if let p = tabManager.goBack() { loadURL(p) }
    }
    func goForward()     { webView?.goForward() }
    func reload()        { webView?.reload(); errorMessage = nil }
    func stopLoading()   { webView?.stopLoading() }
    func loadURL(_ s: String) {
        urlInput = s; guard let u = URL(string: s) else { return }
        webView?.load(URLRequest(url: u))
    }

    // ズーム
    func zoomIn()  { zoomLevel = min(zoomLevel + 0.25, 3.0); applyZoom() }
    func zoomOut() { zoomLevel = max(zoomLevel - 0.25, 0.5); applyZoom() }
    func zoomReset() { zoomLevel = 1.0; applyZoom() }
    private func applyZoom() {
        webView?.evaluateJavaScript("document.body.style.zoom='\(zoomLevel)';")
    }

    // ページ内検索
    func findInPage() {
        guard !findQuery.isEmpty else { clearFind(); return }
        let esc = findQuery.replacingOccurrences(of: "'", with: "\\'")
        let js = """
        window.getSelection().removeAllRanges();
        var count=0,txt='\(esc)',body=document.body.innerHTML;
        document.body.innerHTML=body.replace(new RegExp(txt,'gi'),m=>{count++;return '<mark style="background:#FFE066;color:#000">'+m+'</mark>';});
        count;
        """
        webView?.evaluateJavaScript(js) { [weak self] res, _ in
            Task { @MainActor in self?.findResultCount = (res as? Int) ?? 0 }
        }
    }
    func clearFind() {
        findQuery = ""; findResultCount = 0
        webView?.evaluateJavaScript("document.body.innerHTML=document.body.innerHTML.replace(/<mark[^>]*>(.*?)<\\/mark>/gi,'$1');")
    }

    // ブックマーク
    func addBookmark() {
        guard !pageTitle.isEmpty, let u = URL(string: urlInput) else { return }
        let bm = VxBookmark(title: pageTitle, url: urlInput, favicon: u.host ?? "")
        if !bookmarks.contains(where: { $0.url == urlInput }) { bookmarks.insert(bm, at: 0) }
    }
    var isBookmarked: Bool { bookmarks.contains { $0.url == urlInput } }

    // キーボード入力注入
    func injectKey(_ key: String) {
        switch key {
        case "⌫","BACKSPACE": if !urlInput.isEmpty { urlInput.removeLast() }
        case "SPACE","SPC":   urlInput += " "
        case "⏎","RETURN","ENTER": break
        default: urlInput += key
        }
    }
    func injectKeyToPage(_ key: String) {
        let js: String
        switch key {
        case "⌫","BACKSPACE":
            js="var e=document.activeElement;if(e&&e.value!=undefined){var s=e.selectionStart;if(s>0){e.value=e.value.slice(0,s-1)+e.value.slice(s);e.selectionStart=e.selectionEnd=s-1;}}"
        case "⏎","RETURN","ENTER":
            js="var e=document.activeElement;if(e){if(e.form)e.form.submit();else e.dispatchEvent(new KeyboardEvent('keydown',{key:'Enter',keyCode:13,bubbles:true}));}"
        default:
            let esc=key.replacingOccurrences(of:"'",with:"\\'")
            js="document.execCommand('insertText',false,'\(esc)');"
        }
        webView?.evaluateJavaScript(js)
    }
    func evalJS(_ script: String) { webView?.evaluateJavaScript(script) }
}

// MARK: - VxBookmark
struct VxBookmark: Identifiable, Codable {
    let id:     UUID
    let title:  String
    let url:    String
    let favicon: String

    init(title: String, url: String, favicon: String) {
        self.id = UUID(); self.title = title; self.url = url; self.favicon = favicon
    }

    static let defaults: [VxBookmark] = [
        VxBookmark(title: "Google",   url: "https://www.google.com",   favicon: "google.com"),
        VxBookmark(title: "YouTube",  url: "https://www.youtube.com",  favicon: "youtube.com"),
        VxBookmark(title: "GitHub",   url: "https://github.com",       favicon: "github.com"),
        VxBookmark(title: "Wikipedia",url: "https://www.wikipedia.org",favicon: "wikipedia.org"),
        VxBookmark(title: "X",        url: "https://x.com",            favicon: "x.com"),
    ]
}
