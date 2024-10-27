import { LoaderProps, Stack, Loader as MantineLoader } from "@mantine/core";
import { FC } from "react";

type Props = LoaderProps;

export const Loader: FC<Props> = (props) => {
  return (
    <Stack
      align="center"
      justify="center"
      sx={{
        flexGrow: 1,
      }}
    >
      <MantineLoader {...props} />
    </Stack>
  );
};
