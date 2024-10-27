import { model, Schema, Types } from "mongoose";

export interface StateDocument {
  key?: Types.ObjectId;
  status: "WAITING" | "PROCESSING" | "COMPLETED" | "EXPIRED";
  platform: "WINDOWS" | "MACOS" | "ANDROID" | "IOS";
  stage: number;
  token: string;
  ip: string;
  hwid: string;
  userAgent: string;
  createdAt: Date;
  updatedAt: Date;
  expiresAt: Date;
}

export const StateModel = model<StateDocument>(
  "State",
  new Schema(
    {
      key: { type: Schema.Types.ObjectId, ref: "Key" },
      status: {
        type: String,
        required: true,
      },
      platform: {
        type: String,
        required: true,
      },
      stage: {
        type: Number,
        required: true,
      },
      token: {
        type: String,
        required: true,
      },
      ip: {
        type: String,
        required: true,
      },
      hwid: {
        type: String,
        required: true,
      },
      userAgent: {
        type: String,
        required: true,
      },
      createdAt: {
        type: Date,
        required: true,
      },
      updatedAt: {
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
