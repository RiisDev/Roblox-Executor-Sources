import { model, Schema } from "mongoose";

export interface releaseDocument {
  latest: boolean;
  type: string;
  version: string;
  status: "PATCHED" | "UPDATING" | "WORKING";
  platform: "WINDOWS" | "MACOS" | "ANDROID" | "IOS";
  createdAt: Date;
}

export const ReleaseModel = model<releaseDocument>(
  "release",
  new Schema(
    {
      latest: {
        type: Boolean,
        required: true,
      },
      type: {
        type: String,
        required: true,
      },
      version: {
        type: String,
        required: true,
      },
      status: {
        type: String,
        required: true,
      },
      platform: {
        type: String,
        required: true,
      },
      createdAt: {
        type: Date,
        required: true,
      },
    },
    {
      autoCreate: false,
    }
  )
);
