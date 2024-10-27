import { model, Schema, Types } from "mongoose";

export interface _userDocument {
  email: string;
  username: string;
  password: string;
  roles: ("BANNED" | "VERIFIED" | "PREMIUM" | "ADMIN")[];
  stripeCustomerId?: string;
  createdAt: Date;
}

export interface userDocument {
  state?: Types.ObjectId;
  status: "ACTIVE" | "EXPIRED" | "DISABLED";
  value: string;
  expiresAt: Date;
  createdAt: Date;
}

export const UserModel = model<userDocument>(
  "user",
  new Schema(
    {
      state: { type: Schema.Types.ObjectId, ref: "State" },
      status: {
        type: String,
        required: true,
      },
      value: {
        type: String,
        required: true,
      },
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
