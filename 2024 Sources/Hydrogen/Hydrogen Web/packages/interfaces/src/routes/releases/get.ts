import { ApiResponse } from "../../common";
import { ReleaseObject } from "../../objects/Release";

export interface GetReleasesRoute {
  Reply: ApiResponse<ReleaseObject[]>;
}
