import { model, Schema } from "mongoose";

export interface KeyDocument {
  state: {
    hwid: string;
  };
  status: "ACTIVE" | "EXPIRED" | "DISABLED";
  value: string;
  expiresAt: Date;
  createdAt: Date;
}

export const KeyModel = model<KeyDocument>(
  "Key",
  new Schema(
    {
      expiresAt: {
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
