import { Group, Text, useMantineTheme } from "@mantine/core";
import { NextLink } from "@mantine/next";
import { FC } from "react";

export const Legal: FC = () => {
  const theme = useMantineTheme();

  return (
    <Group>
      <Text
        size="sm"
        color={theme.colors.gray[4]}
        component={NextLink}
        href="/legal/terms-conditions"
        sx={{
          ":hover": {
            color: theme.colors.gray[5],
          },
        }}
      >
        Terms of Service
      </Text>
      <Text
        size="sm"
        color={theme.colors.gray[4]}
        component={NextLink}
        href="/legal/privacy"
        sx={{
          ":hover": {
            color: theme.colors.gray[5],
          },
        }}
      >
        Privacy Policy
      </Text>
    </Group>
  );
};
