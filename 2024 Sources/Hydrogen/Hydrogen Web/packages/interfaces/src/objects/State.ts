export interface StateObject {
  stage: number;
  url?: string;
  key?: {
    value: string;
    expiresAt: Date;
  };
}
