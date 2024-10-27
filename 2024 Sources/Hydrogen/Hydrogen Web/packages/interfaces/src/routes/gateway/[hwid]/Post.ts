import { ApiResponse } from "../../../common";
import { StateObject } from "../../../objects";

export interface PostGatewayRoute {
  Params: {
    hwid: string;
  };
  Body: {
    token: string;
    platform: "WINDOWS" | "MACOS" | "ANDROID" | "IOS";
  };
  Headers: {
    "x-real-ip": string;
  };
  Reply: ApiResponse<StateObject>;
}
