import { model, Schema, Types } from "mongoose";

export interface KeyDocument {
  state?: Types.ObjectId;
  status: "ACTIVE" | "EXPIRED" | "DISABLED";
  platform: "WINDOWS" | "MACOS" | "ANDROID" | "IOS";
  value: string;
  hwid: string;
  createdAt: Date;
  expiresAt: Date;
}

export const KeyModel = model<KeyDocument>(
  "Key",
  new Schema(
    {
      state: { type: Schema.Types.ObjectId, ref: "State" },
      status: {
        type: String,
        required: true,
      },
      platform: {
        type: String,
        required: true,
      },
      value: {
        type: String,
        required: true,
      },
      hwid: {
        type: String,
        required: true,
      },
      createdAt: {
        type: Date,
        required: true,
      },
      expiresAt: {
        type: Date,
        required: true,
      },
    },
    {
      autoCreate: false,
    }
  )
);
