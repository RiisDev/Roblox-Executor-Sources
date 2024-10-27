//
//  ContentView.swift
//  Hydrogen-GUI
//
//  Created by Braeden Weaver on 5/28/22.
//

import Foundation
import SwiftUI

extension Color {
    init(hex: UInt, alpha: Double = 1) {
        self.init(
            .sRGB,
            red: Double((hex >> 16) & 0xff) / 255,
            green: Double((hex >> 08) & 0xff) / 255,
            blue: Double((hex >> 00) & 0xff) / 255,
            opacity: alpha
        )
    }
}

extension Bundle {
    var releaseVersionNumber: String? {
        return infoDictionary?["CFBundleShortVersionString"] as? String
    }
    var buildVersionNumber: String? {
        return infoDictionary?["CFBundleVersion"] as? String
    }
}

func inject(dylib_path: URL) {
    let process = Process()
    let pipe = Pipe()

    process.standardOutput = pipe
    process.executableURL = Bundle.main.url(forResource: "insert_dylib", withExtension: "")
    process.arguments = ["--strip-codesig", "--all-yes", dylib_path.relativePath, "/Applications/Roblox.app/Contents/MacOS/RobloxPlayerBack", "/Applications/Roblox.app/Contents/MacOS/RobloxPlayer"]

    try! process.run()
    //process.waitUntilExit() // you might need this

    let data = pipe.fileHandleForReading.readDataToEndOfFile()

    guard let standardOutput = String(data: data, encoding: .utf8) else {
        FileHandle.standardError.write(Data("Error in reading standard output data".utf8))
        fatalError()
    }
    
    print(standardOutput)

}

func loadHydrogenBinarys() {
    let dylibPath = Bundle.main.url(forResource: "libHydrogen", withExtension: "dylib")!
    do {
        let dydata = try Data(contentsOf: dylibPath)
        let dyfileURL = URL(fileURLWithPath: "/Applications/Roblox.app/Contents/MacOS/").appendingPathComponent("libHydrogen.dylib")
        try dydata.write(to: dyfileURL)
                
        if (FileManager.default.fileExists(atPath: "/Applications/Roblox.app/Contents/MacOS/RobloxPlayerBack")) {
            try? FileManager.default.removeItem(atPath: "/Applications/Roblox.app/Contents/MacOS/RobloxPlayer")
            try? FileManager.default.copyItem(atPath: "/Applications/Roblox.app/Contents/MacOS/RobloxPlayerBack",
                                         toPath: "/Applications/Roblox.app/Contents/MacOS/RobloxPlayer")
            try? FileManager.default.removeItem(atPath: "/Applications/Roblox.app/Contents/MacOS/RobloxPlayerBack")
        }
        try FileManager.default.copyItem(atPath: "/Applications/Roblox.app/Contents/MacOS/RobloxPlayer", toPath: "/Applications/Roblox.app/Contents/MacOS/RobloxPlayerBack")
        inject(dylib_path: dyfileURL)
    } catch {
        print(error)
    }
}

struct ContentView: View {
    @State var key = ""
    @State var authenticated = false
    @State var expiryTime: Date = Date()
    
    var serialNumber: String? {
        let platformExpert = IOServiceGetMatchingService(kIOMainPortDefault, IOServiceMatching("IOPlatformExpertDevice") )

      guard platformExpert > 0 else {
        return nil
      }

      guard let serialNumber = (IORegistryEntryCreateCFProperty(platformExpert, kIOPlatformSerialNumberKey as CFString, kCFAllocatorDefault, 0).takeUnretainedValue() as? String)?.trimmingCharacters(in: CharacterSet.whitespacesAndNewlines) else {
        return nil
      }

      IOObjectRelease(platformExpert)

      return serialNumber
    }
    
    var hwid: String? {
        if let uuid = serialNumber {
            do {
                let attributes = try manager.attributesOfFileSystem(forPath: "/")
                if let fs_ver = attributes[FileAttributeKey.systemNumber] {
                    if let fs_size = attributes[FileAttributeKey.systemSize] {
                        let ver = String(describing: fs_ver)
                        let size = String(describing: fs_size)
                        let hwid = uuid +
                         String("-") + ver + String("-") + size
                        return hwid
                    }
                }
            } catch {
                print("Missing rootfs? Lol")
            }
        }
        return nil
    }
    
    let manager = FileManager.init()
    
    var body: some View {
        if !authenticated {
            VStack(alignment: .leading) {
                GeometryReader { gp in
                    Image("Splash Image")
                        .resizable()
                        .aspectRatio(contentMode: .fit)
                        .frame(width: 600, height: 300)
                        .shadow(color: Color.white.opacity(0.3), radius: 5, x: 5, y: 5)
                        .position(x: gp.size.width / 2, y: gp.size.height / 2)
                }
                Link(destination: URL(string: "https://gateway.hydrogen.sh/" + hwid! + "?platform=MACOS")!, label: {
                    HStack {
                        Spacer()
                        Image(systemName: "key.icloud.fill")
                            .foregroundColor(Color.white)
                        Text("Get Key")
                            .foregroundColor(Color.white)
                        Spacer()
                    }
                    .padding(.vertical, 8)
                    .padding(.horizontal)
                    .background(Color(hex: 0x8DC1F0))
                    
                })
                .padding(.horizontal)
                Button(action: {
                    if let key = NSPasteboard.general.string(forType: .string) {
                        print("https://api.hydrogen.sh/gateway/auth?key=" + key
                              + "&hwid=" + hwid!)
                        let url = URL(string: "https://api.hydrogen.sh/gateway/auth?key=" + key
                            + "&hwid=" + hwid!)!
                        let task = URLSession.shared.dataTask(with: url) {(data, response, error) in
                            guard let data = data else { return }
                            let resp = String(data: data, encoding: .utf8)!
                            print(resp)
                            do {
                                let dictionary = try JSONSerialization.jsonObject(with: data, options:.mutableContainers) as? [String: Any]
                                let jsData = dictionary!["data"] as? [String: Any]
                                let expires = jsData!["expiresAt"] as! String
                                
                                let dateFormatter = DateFormatter()
                                dateFormatter.dateFormat = "yyyy-MM-dd'T'HH:mm:ss.SSSZ"
                                guard let expiryDate = dateFormatter.date(from: expires) else {
                                   fatalError("ERROR: Date conversion failed due to mismatched format.")
                                }
                                
                                let curDate = Date()
                                if (curDate < expiryDate) {
                                    expiryTime = expiryDate
                                    authenticated = true
                                }
                            } catch {
                                print("Failed to parse json data")
                            }
                            
                        }
                        task.resume()
                    }
                }, label: {
                    HStack {
                        Spacer()
                        Image(systemName: "person.badge.key.fill")
                            .foregroundColor(Color.white)
                        Text("Authenticate using Key from clipboard")
                            .foregroundColor(Color.white)
                        Spacer()
                    }
                    .padding(.vertical, 8)
                    .background(Color(hex: 0x8DC1F0))
                })
                .buttonStyle(PlainButtonStyle())
                .padding(.vertical, 8)
                .padding(.horizontal)
                .foregroundColor(Color.white)
                .onAppear(perform: {
                    loadHydrogenBinarys()
                })
            }
            .frame(maxWidth: .infinity, maxHeight: .infinity, alignment: .topLeading)
            .background(Color.black.opacity(0.8))
            .onAppear(perform: {
                let url = URL(string: "https://api.hydrogen.sh/releases/MACOS")!
                let task = URLSession.shared.dataTask(with: url) {(data, response, error) in
                    guard let data = data else { return }
                    let resp = String(data: data, encoding: .utf8)!
                    print(resp)
                    do {
                        let dictionary = try JSONSerialization.jsonObject(with: data, options:.mutableContainers) as? [String: Any]
                        let jsData = dictionary!["data"] as? [String: Any]
                        let version = jsData!["version"] as! String
                        if (version != Bundle.main.releaseVersionNumber!) {
                            DispatchQueue.main.async {
                                let alert = NSAlert()
                                alert.messageText = "Hydrogen is out of date!"
                                alert.informativeText = "Hydrogen is out of date. Please download the latest version at https://www.hydrogen.sh/"
                                alert.addButton(withTitle: "OK")
                                let logoIcon = NSImage(named: .logo)
                                alert.icon = logoIcon
                                alert.runModal()
                                exit(0)
                            }
                        }
                    } catch {
                        print("Failed to parse json data")
                    }
                    
                }
                task.resume()
            })
        } else {
            ExecutionView(expiryTime: $expiryTime)
        }
        //ExecutionView(expiryTime: $expiryTime)
    }
}

struct ContentView_Previews: PreviewProvider {
    static var previews: some View {
        ContentView()
            .frame(maxWidth: .infinity, maxHeight: .infinity)
    }
}
