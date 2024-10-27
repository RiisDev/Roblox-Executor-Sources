import { ApiResponse } from "../../../common";
import { StateObject } from "../../../objects";

export interface GetGatewayRoute {
  Params: {
    hwid: string;
  };
  Reply: ApiResponse<StateObject>;
}
