export interface ReleaseObject {
  url: string;
  version: string;
  status: "PATCHED" | "UPDATING" | "WORKING";
  platform: "WINDOWS" | "MACOS" | "ANDROID" | "IOS";
  createdAt: Date;
}
