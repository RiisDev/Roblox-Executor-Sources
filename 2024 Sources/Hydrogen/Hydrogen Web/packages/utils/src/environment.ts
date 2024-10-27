export const environmentUtil = (): "development" | "staging" | "production" => {
  const envOptions = ["development", "staging", "production"];

  for (const environment of envOptions) {
    if (process.env.NODE_ENV?.toLowerCase() === environment)
      return environment as any;
  }

  return "development";
};
