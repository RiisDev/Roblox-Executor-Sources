import { ApiResponse } from "../../../common";
import { AuthObject } from "../../../objects/Auth";

export interface GetAuthRoute {
  Querystring: {
    key: string;
    hwid: string;
  };
  Reply: ApiResponse<AuthObject>;
}
