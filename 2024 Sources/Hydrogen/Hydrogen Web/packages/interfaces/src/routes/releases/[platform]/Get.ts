import { ApiResponse } from "../../../common";
import { ReleaseObject } from "../../../objects/Release";

export interface GetReleaseRoute {
  Params: {
    platform: string;
  };
  Reply: ApiResponse<ReleaseObject>;
}
