import SwiftUI
import WebKit

// MARK: - Design Tokens (モダン・フレッシュ)
enum VxDS {
    // Primary — vivid indigo-to-violet
    static let p1  = Color(red: 99/255,  green: 102/255, blue: 241/255) // indigo-500
    static let p2  = Color(red: 139/255, green: 92/255,  blue: 246/255) // violet-500
    static let p3  = Color(red: 168/255, green: 85/255,  blue: 247/255) // purple-500

    // Surface
    static let bg      = Color(red: 250/255, green: 250/255, blue: 255/255)
    static let surface = Color.white
    static let card    = Color(red: 244/255, green: 244/255, blue: 255/255)
    static let divider = Color(red: 226/255, green: 226/255, blue: 243/255)

    // Text
    static let textPrimary   = Color(red: 17/255,  green: 24/255,  blue: 39/255)
    static let textSecondary = Color(red: 107/255, green: 114/255, blue: 128/255)
    static let textMuted     = Color(red: 156/255, green: 163/255, blue: 175/255)

    // Status
    static let success = Color(red: 34/255,  green: 197/255, blue: 94/255)
    static let warning = Color(red: 251/255, green: 191/255, blue: 36/255)
    static let error   = Color(red: 239/255, green: 68/255,  blue: 68/255)

    // Gradient
    static let grad = LinearGradient(colors: [p1, p2], startPoint: .leading, endPoint: .trailing)
    static let gradV = LinearGradient(colors: [p1, p3], startPoint: .top, endPoint: .bottom)
}

// MARK: - VxSpatialBrowserView (メインコンテナ)
struct VxSpatialBrowserView: View {
    @StateObject var tabManager = VxTabManager()
    @StateObject var vm         = VxBrowserViewModel()

    var body: some View {
        VStack(spacing: 0) {
            VxToolbar()
                .environmentObject(tabManager)
                .environmentObject(vm)

            VxBookmarkBar()
                .environmentObject(tabManager)
                .environmentObject(vm)

            // Progress bar
            if vm.isLoading {
                GeometryReader { g in
                    ZStack(alignment: .leading) {
                        Rectangle().fill(VxDS.divider).frame(height: 3)
                        Rectangle()
                            .fill(VxDS.grad)
                            .frame(width: g.size.width * vm.loadProgress, height: 3)
                            .animation(.linear(duration: 0.2), value: vm.loadProgress)
                    }
                }.frame(height: 3)
            } else {
                Divider().background(VxDS.divider)
            }

            // Find bar
            if vm.isFindBarOpen { VxFindBar().environmentObject(vm) }

            // Content
            ZStack {
                if vm.isNewTabPage {
                    VxNewTabPage()
                        .environmentObject(tabManager)
                        .environmentObject(vm)
                } else {
                    VxWebView()
                        .environmentObject(tabManager)
                        .environmentObject(vm)
                }
                if let err = vm.errorMessage { VxErrorPage(message: err).environmentObject(vm) }
            }
            .frame(maxWidth: .infinity, maxHeight: .infinity)
        }
        .background(VxDS.bg)
        .frame(width: 1024, height: 720)
        .clipShape(RoundedRectangle(cornerRadius: 20))
        .shadow(color: VxDS.p1.opacity(0.18), radius: 40, x: 0, y: 12)
        .onReceive(tabManager.$activeTabID) { _ in
            guard let tab = tabManager.activeTab else { return }
            vm.urlInput = tab.url
            vm.isNewTabPage = tab.url.isEmpty || tab.url == "newtab"
        }
    }
}

// MARK: - VxToolbar
struct VxToolbar: View {
    @EnvironmentObject var tabManager: VxTabManager
    @EnvironmentObject var vm: VxBrowserViewModel
    @State private var isURLEditing = false

    var body: some View {
        VStack(spacing: 0) {
            // Tab bar
            HStack(spacing: 0) {
                ScrollView(.horizontal, showsIndicators: false) {
                    HStack(spacing: 4) {
                        ForEach(tabManager.tabs) { tab in
                            VxTabChip(tab: tab)
                                .environmentObject(tabManager)
                                .environmentObject(vm)
                        }
                    }.padding(.horizontal, 12).padding(.top, 10)
                }
                Spacer()
                HStack(spacing: 6) {
                    toolbarBtn("plus") { tabManager.newTab() }
                    toolbarBtn("lock.shield") { tabManager.newTab(isPrivate: true) }
                }.padding(.trailing, 12).padding(.top, 10)
            }
            .frame(height: 48)

            // Nav bar
            HStack(spacing: 8) {
                // Back / Forward
                navBtn("chevron.left",  enabled: vm.canGoBack)    { vm.goBack(tabManager: tabManager) }
                navBtn("chevron.right", enabled: vm.canGoForward) { vm.goForward() }

                // Reload/Stop
                Button { vm.isLoading ? vm.stopLoading() : vm.reload() } label: {
                    Image(systemName: vm.isLoading ? "xmark" : "arrow.clockwise")
                        .font(.system(size: 13, weight: .medium))
                        .foregroundColor(VxDS.textSecondary)
                        .frame(width: 32, height: 32)
                        .background(VxDS.card)
                        .clipShape(Circle())
                }
                .buttonStyle(.plain)

                // URL bar
                HStack(spacing: 8) {
                    // Lock / Security
                    Image(systemName: vm.isSecure ? "lock.fill" : "exclamationmark.triangle.fill")
                        .font(.system(size: 11))
                        .foregroundColor(vm.isSecure ? VxDS.success : VxDS.warning)

                    TextField("Search or enter URL", text: $vm.urlInput, onEditingChanged: { e in
                        isURLEditing = e; vm.isURLBarFocused = e
                    })
                    .font(.system(size: 13, weight: .medium))
                    .foregroundColor(VxDS.textPrimary)
                    .textFieldStyle(.plain)
                    .autocorrectionDisabled()
                    .onSubmit { vm.navigate(tabManager: tabManager) }

                    if !vm.urlInput.isEmpty && isURLEditing {
                        Button { vm.urlInput = "" } label: {
                            Image(systemName: "xmark.circle.fill")
                                .foregroundColor(VxDS.textMuted)
                                .font(.system(size: 13))
                        }.buttonStyle(.plain)
                    }

                    // Bookmark star
                    Button { vm.addBookmark() } label: {
                        Image(systemName: vm.isBookmarked ? "star.fill" : "star")
                            .font(.system(size: 13))
                            .foregroundColor(vm.isBookmarked ? VxDS.warning : VxDS.textMuted)
                    }.buttonStyle(.plain)
                }
                .padding(.horizontal, 12)
                .padding(.vertical, 8)
                .background(isURLEditing ? VxDS.surface : VxDS.card)
                .clipShape(Capsule())
                .overlay(Capsule().stroke(
                    isURLEditing ? VxDS.p1.opacity(0.6) : VxDS.divider, lineWidth: 1.5
                ))
                .shadow(color: isURLEditing ? VxDS.p1.opacity(0.12) : .clear, radius: 8)
                .animation(.easeInOut(duration: 0.2), value: isURLEditing)

                // Zoom
                HStack(spacing: 2) {
                    zoomBtn("minus") { vm.zoomOut() }
                    Button { vm.zoomReset() } label: {
                        Text("\(Int(vm.zoomLevel * 100))%")
                            .font(.system(size: 10, weight: .semibold))
                            .foregroundColor(vm.zoomLevel == 1.0 ? VxDS.textMuted : VxDS.p1)
                            .frame(width: 36)
                    }.buttonStyle(.plain)
                    zoomBtn("plus") { vm.zoomIn() }
                }
                .padding(.horizontal, 4)
                .padding(.vertical, 4)
                .background(VxDS.card)
                .clipShape(RoundedRectangle(cornerRadius: 8))

                // Find
                toolbarBtn("doc.text.magnifyingglass") { vm.isFindBarOpen.toggle() }
            }
            .padding(.horizontal, 12)
            .padding(.bottom, 10)
        }
        .background(VxDS.surface)
        .overlay(Divider().background(VxDS.divider), alignment: .bottom)
    }

    private func toolbarBtn(_ icon: String, action: @escaping () -> Void) -> some View {
        Button(action: action) {
            Image(systemName: icon)
                .font(.system(size: 13, weight: .medium))
                .foregroundColor(VxDS.textSecondary)
                .frame(width: 32, height: 32)
                .background(VxDS.card)
                .clipShape(RoundedRectangle(cornerRadius: 8))
        }.buttonStyle(.plain)
    }

    private func navBtn(_ icon: String, enabled: Bool, action: @escaping () -> Void) -> some View {
        Button(action: action) {
            Image(systemName: icon)
                .font(.system(size: 13, weight: .semibold))
                .foregroundColor(enabled ? VxDS.textPrimary : VxDS.textMuted)
                .frame(width: 32, height: 32)
                .background(enabled ? VxDS.card : Color.clear)
                .clipShape(Circle())
        }
        .buttonStyle(.plain)
        .disabled(!enabled)
    }

    private func zoomBtn(_ icon: String, action: @escaping () -> Void) -> some View {
        Button(action: action) {
            Image(systemName: icon)
                .font(.system(size: 10, weight: .bold))
                .foregroundColor(VxDS.textSecondary)
                .frame(width: 24, height: 24)
        }.buttonStyle(.plain)
    }
}

// MARK: - VxTabChip
struct VxTabChip: View {
    let tab: VxTab
    @EnvironmentObject var tabManager: VxTabManager
    @EnvironmentObject var vm: VxBrowserViewModel
    var isActive: Bool { tabManager.activeTabID == tab.id }

    var body: some View {
        HStack(spacing: 6) {
            ZStack {
                if tab.isLoading {
                    ProgressView().scaleEffect(0.5).tint(VxDS.p1)
                } else {
                    Image(systemName: tab.isPrivate ? "lock.shield.fill" : "globe")
                        .font(.system(size: 10))
                        .foregroundStyle(isActive ? VxDS.grad : LinearGradient(colors: [VxDS.textMuted], startPoint: .top, endPoint: .bottom))
                }
            }.frame(width: 14, height: 14)

            Text(tab.summary)
                .font(.system(size: 12, weight: isActive ? .semibold : .regular))
                .foregroundColor(isActive ? VxDS.textPrimary : VxDS.textSecondary)
                .lineLimit(1)
                .frame(maxWidth: 130)

            if tabManager.tabs.count > 1 {
                Button { tabManager.closeTab(id: tab.id) } label: {
                    Image(systemName: "xmark").font(.system(size: 8, weight: .bold))
                        .foregroundColor(VxDS.textMuted)
                }.buttonStyle(.plain)
            }
        }
        .padding(.horizontal, 12)
        .padding(.vertical, 6)
        .background(
            RoundedRectangle(cornerRadius: 10)
                .fill(isActive ? VxDS.surface : Color.clear)
                .shadow(color: isActive ? VxDS.p1.opacity(0.10) : .clear, radius: 6, y: 2)
        )
        .overlay(
            RoundedRectangle(cornerRadius: 10)
                .stroke(isActive ? VxDS.p1.opacity(0.25) : Color.clear, lineWidth: 1)
        )
        .onTapGesture { tabManager.switchTo(id: tab.id) }
        .animation(.spring(response: 0.25), value: isActive)
    }
}

// MARK: - VxBookmarkBar
struct VxBookmarkBar: View {
    @EnvironmentObject var tabManager: VxTabManager
    @EnvironmentObject var vm: VxBrowserViewModel

    var body: some View {
        if vm.showBookmarkBar {
            ScrollView(.horizontal, showsIndicators: false) {
                HStack(spacing: 4) {
                    ForEach(vm.bookmarks) { bm in
                        Button {
                            vm.urlInput = bm.url
                            vm.isNewTabPage = false
                            vm.navigate(tabManager: tabManager)
                        } label: {
                            HStack(spacing: 5) {
                                Image(systemName: "globe")
                                    .font(.system(size: 10))
                                    .foregroundColor(VxDS.p1)
                                Text(bm.title)
                                    .font(.system(size: 11, weight: .medium))
                                    .foregroundColor(VxDS.textSecondary)
                            }
                            .padding(.horizontal, 10)
                            .padding(.vertical, 5)
                            .background(VxDS.card)
                            .clipShape(Capsule())
                        }
                        .buttonStyle(.plain)
                    }
                }
                .padding(.horizontal, 12)
                .padding(.vertical, 5)
            }
            .frame(height: 34)
            .background(VxDS.surface)
            .overlay(Divider().background(VxDS.divider), alignment: .bottom)
        }
    }
}

// MARK: - VxFindBar
struct VxFindBar: View {
    @EnvironmentObject var vm: VxBrowserViewModel

    var body: some View {
        HStack(spacing: 10) {
            Image(systemName: "magnifyingglass")
                .font(.system(size: 12))
                .foregroundColor(VxDS.textMuted)
            TextField("Find in page…", text: $vm.findQuery)
                .font(.system(size: 13))
                .foregroundColor(VxDS.textPrimary)
                .textFieldStyle(.plain)
                .onChange(of: vm.findQuery) { _ in vm.findInPage() }
            if vm.findResultCount > 0 {
                Text("\(vm.findResultCount) found")
                    .font(.system(size: 11, weight: .medium))
                    .foregroundColor(VxDS.success)
            }
            Spacer()
            Button { vm.clearFind(); vm.isFindBarOpen = false } label: {
                Image(systemName: "xmark.circle.fill")
                    .foregroundColor(VxDS.textMuted)
            }.buttonStyle(.plain)
        }
        .padding(.horizontal, 14)
        .padding(.vertical, 8)
        .background(VxDS.card)
        .overlay(Divider().background(VxDS.divider), alignment: .bottom)
    }
}

// MARK: - VxNewTabPage
struct VxNewTabPage: View {
    @EnvironmentObject var tabManager: VxTabManager
    @EnvironmentObject var vm: VxBrowserViewModel
    @State private var searchText = ""

    let quickLinks: [(String, String, String)] = [
        ("Google",    "https://google.com",    "magnifyingglass"),
        ("YouTube",   "https://youtube.com",   "play.rectangle.fill"),
        ("GitHub",    "https://github.com",    "chevron.left.forwardslash.chevron.right"),
        ("Wikipedia", "https://wikipedia.org", "books.vertical.fill"),
        ("X / Twitter","https://x.com",        "at"),
        ("Apple",     "https://apple.com",     "apple.logo"),
    ]

    var body: some View {
        ZStack {
            // Mesh-like gradient background
            LinearGradient(
                colors: [VxDS.bg, Color(red: 238/255, green: 238/255, blue: 255/255)],
                startPoint: .topLeading, endPoint: .bottomTrailing
            )
            Circle()
                .fill(VxDS.p1.opacity(0.07))
                .frame(width: 500).blur(radius: 60)
                .offset(x: -150, y: -100)
            Circle()
                .fill(VxDS.p3.opacity(0.07))
                .frame(width: 400).blur(radius: 50)
                .offset(x: 200, y: 150)

            VStack(spacing: 36) {
                VStack(spacing: 8) {
                    Text("Good Day ☀️")
                        .font(.system(size: 28, weight: .bold, design: .rounded))
                        .foregroundStyle(VxDS.grad)
                    Text("Where would you like to go?")
                        .font(.system(size: 14))
                        .foregroundColor(VxDS.textSecondary)
                }

                // Inline search
                HStack(spacing: 10) {
                    Image(systemName: "magnifyingglass")
                        .foregroundColor(VxDS.p1)
                    TextField("Search or enter URL…", text: $searchText)
                        .font(.system(size: 14))
                        .textFieldStyle(.plain)
                        .autocorrectionDisabled()
                        .onSubmit {
                            vm.urlInput = searchText
                            vm.navigate(tabManager: tabManager)
                        }
                }
                .padding(.horizontal, 20)
                .padding(.vertical, 14)
                .background(VxDS.surface)
                .clipShape(RoundedRectangle(cornerRadius: 16))
                .overlay(RoundedRectangle(cornerRadius: 16).stroke(VxDS.p1.opacity(0.3), lineWidth: 1.5))
                .shadow(color: VxDS.p1.opacity(0.10), radius: 12)
                .frame(maxWidth: 560)

                // Quick links grid
                LazyVGrid(columns: Array(repeating: GridItem(.fixed(130), spacing: 16), count: 6), spacing: 16) {
                    ForEach(quickLinks, id: \.1) { name, url, icon in
                        Button {
                            vm.urlInput = url
                            vm.navigate(tabManager: tabManager)
                        } label: {
                            VStack(spacing: 10) {
                                ZStack {
                                    RoundedRectangle(cornerRadius: 16)
                                        .fill(VxDS.surface)
                                        .shadow(color: VxDS.p1.opacity(0.08), radius: 8, y: 4)
                                    Image(systemName: icon)
                                        .font(.system(size: 22, weight: .semibold))
                                        .foregroundStyle(VxDS.grad)
                                }
                                .frame(width: 56, height: 56)
                                Text(name)
                                    .font(.system(size: 11, weight: .medium))
                                    .foregroundColor(VxDS.textSecondary)
                            }
                        }
                        .buttonStyle(.plain)
                    }
                }

                // Bookmarks
                if !vm.bookmarks.isEmpty {
                    VStack(alignment: .leading, spacing: 10) {
                        Text("Bookmarks")
                            .font(.system(size: 12, weight: .semibold))
                            .foregroundColor(VxDS.textMuted)
                        ScrollView(.horizontal, showsIndicators: false) {
                            HStack(spacing: 8) {
                                ForEach(vm.bookmarks) { bm in
                                    Button {
                                        vm.urlInput = bm.url
                                        vm.navigate(tabManager: tabManager)
                                    } label: {
                                        HStack(spacing: 6) {
                                            Image(systemName: "bookmark.fill")
                                                .font(.system(size: 10))
                                                .foregroundStyle(VxDS.grad)
                                            Text(bm.title)
                                                .font(.system(size: 12, weight: .medium))
                                                .foregroundColor(VxDS.textPrimary)
                                        }
                                        .padding(.horizontal, 12)
                                        .padding(.vertical, 7)
                                        .background(VxDS.surface)
                                        .clipShape(RoundedRectangle(cornerRadius: 10))
                                        .shadow(color: VxDS.p1.opacity(0.06), radius: 4)
                                    }.buttonStyle(.plain)
                                }
                            }
                        }
                    }
                    .frame(maxWidth: 700)
                }
            }
            .padding(40)
        }
    }
}

// MARK: - VxWebView
struct VxWebView: UIViewRepresentable {
    @EnvironmentObject var tabManager: VxTabManager
    @EnvironmentObject var vm: VxBrowserViewModel

    /// Pinch検知時に番届画像URLを通知するコールバック
    var onImageGrabbed: ((String) -> Void)?

    func makeUIView(context: Context) -> WKWebView {
        let cfg = WKWebViewConfiguration()
        cfg.allowsInlineMediaPlayback = true
        cfg.allowsPictureInPictureMediaPlayback = true

        // WKScriptMessageHandler 登録 (画像Pinch通知用)
        cfg.userContentController.add(context.coordinator, name: "imageGrabbed")

        let wv = WKWebView(frame: .zero, configuration: cfg)
        wv.navigationDelegate = context.coordinator
        wv.uiDelegate = context.coordinator
        wv.allowsBackForwardNavigationGestures = true
        wv.backgroundColor = UIColor(VxDS.bg)
        vm.attachWebView(wv)
        if let tab = tabManager.activeTab, !tab.url.isEmpty, let u = URL(string: tab.url) {
            wv.load(URLRequest(url: u))
        }
        return wv
    }

    func updateUIView(_ uiView: WKWebView, context: Context) {}
    func makeCoordinator() -> Coordinator { Coordinator(vm: vm, tabManager: tabManager, onGrabbed: onImageGrabbed) }

    class Coordinator: NSObject, WKNavigationDelegate, WKUIDelegate, WKScriptMessageHandler {
        let vm: VxBrowserViewModel
        let tabManager: VxTabManager
        let onGrabbed: ((String) -> Void)?

        init(vm: VxBrowserViewModel, tabManager: VxTabManager, onGrabbed: ((String) -> Void)? = nil) {
            self.vm = vm; self.tabManager = tabManager; self.onGrabbed = onGrabbed
        }

        // MARK: - 画像Pinch検知JSをページ完了後に注入
        private func injectImageGrabJS(into webView: WKWebView) {
            let js = """
            (function() {
                if (window.__vxImageGrabInstalled) return;
                window.__vxImageGrabInstalled = true;

                document.querySelectorAll('img').forEach(function(img) {
                    img.addEventListener('touchstart', function(e) {
                        var src = img.currentSrc || img.src;
                        if (src && src.startsWith('http')) {
                            window.webkit.messageHandlers.imageGrabbed.postMessage(src);
                        }
                    }, {passive: true});
                });

                // 将来追加される画像にも対応
                var obs = new MutationObserver(function(muts) {
                    muts.forEach(function(m) {
                        m.addedNodes.forEach(function(n) {
                            if (n.tagName === 'IMG') {
                                n.addEventListener('touchstart', function() {
                                    var src = n.currentSrc || n.src;
                                    if (src) window.webkit.messageHandlers.imageGrabbed.postMessage(src);
                                }, {passive: true});
                            }
                        });
                    });
                });
                obs.observe(document.body, { childList: true, subtree: true });
            })();
            """
            webView.evaluateJavaScript(js)
        }

        // WKScriptMessageHandler
        func userContentController(_ userContentController: WKUserContentController,
                                   didReceive message: WKScriptMessage) {
            if message.name == "imageGrabbed", let url = message.body as? String {
                onGrabbed?(url)
            }
        }

        func webView(_ webView: WKWebView, didStartProvisionalNavigation _: WKNavigation!) {
            Task { @MainActor in
                vm.isLoading = true
                vm.errorMessage = nil
                tabManager.updateActiveTab(isLoading: true)
            }
        }

        func webView(_ webView: WKWebView, didFinish _: WKNavigation!) {
            Task { @MainActor in
                vm.isLoading = false
                let title  = webView.title?.isEmpty == false ? webView.title! : "Untitled"
                let urlStr = webView.url?.absoluteString ?? vm.urlInput
                vm.pageTitle  = title
                vm.urlInput   = urlStr
                vm.isSecure   = urlStr.hasPrefix("https")
                tabManager.updateActiveTab(title: title, url: urlStr, isLoading: false)
            }
            // Pinch機能を注入
            injectImageGrabJS(into: webView)
        }

        func webView(_ webView: WKWebView, didFailProvisionalNavigation _: WKNavigation!, withError error: Error) {
            Task { @MainActor in
                vm.isLoading = false
                vm.errorMessage = error.localizedDescription
                tabManager.updateActiveTab(isLoading: false)
            }
        }
        func webView(_ webView: WKWebView, didFail _: WKNavigation!, withError error: Error) {
            Task { @MainActor in vm.isLoading = false; tabManager.updateActiveTab(isLoading: false) }
        }

        func webView(_ webView: WKWebView, createWebViewWith cfg: WKWebViewConfiguration,
                     for action: WKNavigationAction, windowFeatures: WKWindowFeatures) -> WKWebView? {
            if let url = action.request.url {
                Task { @MainActor in
                    self.tabManager.newTab()
                    self.vm.urlInput = url.absoluteString
                    self.vm.navigate(tabManager: self.tabManager)
                }
            }
            return nil
        }
    }
}

// MARK: - VxErrorPage
struct VxErrorPage: View {
    let message: String
    @EnvironmentObject var vm: VxBrowserViewModel

    var body: some View {
        ZStack {
            VxDS.bg.ignoresSafeArea()
            VStack(spacing: 20) {
                ZStack {
                    Circle().fill(VxDS.error.opacity(0.10)).frame(width: 80, height: 80)
                    Image(systemName: "wifi.exclamationmark")
                        .font(.system(size: 32))
                        .foregroundColor(VxDS.error)
                }
                Text("Can't Open Page")
                    .font(.system(size: 22, weight: .bold, design: .rounded))
                    .foregroundColor(VxDS.textPrimary)
                Text(message)
                    .font(.system(size: 13))
                    .foregroundColor(VxDS.textSecondary)
                    .multilineTextAlignment(.center)
                    .frame(maxWidth: 360)
                Button { vm.reload() } label: {
                    HStack(spacing: 8) {
                        Image(systemName: "arrow.clockwise")
                        Text("Try Again")
                    }
                    .font(.system(size: 14, weight: .semibold))
                    .foregroundColor(.white)
                    .padding(.horizontal, 24)
                    .padding(.vertical, 12)
                    .background(VxDS.grad)
                    .clipShape(Capsule())
                }
                .buttonStyle(.plain)
            }
        }
    }
}
