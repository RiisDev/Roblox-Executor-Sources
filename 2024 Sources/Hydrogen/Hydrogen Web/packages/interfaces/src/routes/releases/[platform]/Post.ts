import { ApiResponse } from "../../../common";
import { ReleaseObject } from "../../../objects/Release";

export interface PostReleaseRoute {
  Params: {
    platform: string;
    version: string;
  };
  Reply: ApiResponse<ReleaseObject>;
}
