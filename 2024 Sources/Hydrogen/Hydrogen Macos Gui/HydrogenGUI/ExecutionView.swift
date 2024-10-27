//
//  ExecutionView.swift
//  luau-M-GUI
//
//  Created by Braeden Weaver on 11/6/22.
//

import SwiftUI

struct Scripts: View {
    var body: some View {
       Text("Scripts will go here!")
    }
}

struct ExecutionView: View {
    @State private var selectedView: Int? = 0
    @Binding var expiryTime: Date
    
    var body: some View {
        NavigationView {
            List {
                HStack {
                    Spacer()
                        .frame(maxWidth: 30)
                    Image("Hydrogen")
                        .resizable()
                        .scaledToFit()
                        .frame(width: 50, height: 50)
                }
                Divider()
                NavigationLink(destination: CodeEditorView(expiryTime: $expiryTime), tag: 0, selection: self.$selectedView) {
                    Image(systemName: "square.and.pencil")
                        .foregroundColor(Color(.white))
                    Text("Script Editor")
                }
                NavigationLink(destination: Scripts(), tag: 1, selection: self.$selectedView) {
                    Image(systemName: "shippingbox.fill")
                        .foregroundColor(Color(.white))
                    Text("Script Hub")
                }
                NavigationLink(destination: SettingsView(), tag: 2, selection: self.$selectedView) {
                    Image(systemName: "gearshape.fill")
                        .foregroundColor(Color(.white))
                    Text("Settings")
                }
            }
        }.onAppear {
            DispatchQueue.main.asyncAfter(deadline: .now() + 0.1) {
                guard let nsSplitView = findNSSplitVIew(view: NSApp.windows.first?.contentView), let controller = nsSplitView.delegate as? NSSplitViewController else {
                         return
                }
                controller.splitViewItems.first?.canCollapse = true
                controller.splitViewItems.first?.minimumThickness = 150
                controller.splitViewItems.first?.maximumThickness = 150
            }
        }.toolbar {
            ToolbarItem(placement: .navigation) {
                Button(action: toggleSidebar, label: { // 1
                    Image(systemName: "sidebar.leading")
                })
            }
        }
    }
    
    private func findNSSplitVIew(view: NSView?) -> NSSplitView? {
            var queue = [NSView]()
            if let root = view {
                queue.append(root)
            }
            while !queue.isEmpty {
                let current = queue.removeFirst()
                if current is NSSplitView {
                    return current as? NSSplitView
                }
                for subview in current.subviews {
                    queue.append(subview)
                }
            }
            return nil
    }
    
    private func toggleSidebar() { // 2
        NSApp.keyWindow?.firstResponder?.tryToPerform(#selector(NSSplitViewController.toggleSidebar(_:)), with: nil)
    }
}

/*struct ExecutionView_Previews: PreviewProvider {
    static var previews: some View {
        var isPlaying: Binding<Date>
        ExecutionView(expiryTime: isPlaying)
    }
}*/
