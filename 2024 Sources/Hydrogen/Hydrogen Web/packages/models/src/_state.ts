import { model, Schema } from "mongoose";

export interface StateDocument {
  status: "WAITING" | "PROCESSING" | "COMPLETED" | "EXPIRED";
  platform: "WINDOWS" | "MACOS" | "ANDROID" | "IOS";
  stage: number;
  token: string;
  hwid: string;
  ip: string;
  userAgent: string;
  expiresAt: Date;
  updatedAt: Date;
  createdAt: Date;
}

export const StateModel = model<StateDocument>(
  "State",
  new Schema(
    {
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
      hwid: {
        type: String,
        required: true,
      },
      ip: {
        type: String,
        required: true,
      },
      userAgent: {
        type: String,
        required: true,
      },
      expiresAt: {
        type: Date,
        required: true,
      },
      updatedAt: {
        type: Date,
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
