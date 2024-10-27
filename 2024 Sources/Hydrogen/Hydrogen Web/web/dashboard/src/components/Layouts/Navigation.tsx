import { Group, MediaQuery, Text } from "@mantine/core";
import { NextLink } from "@mantine/next";
import { Logo } from "components/Logo";
import { FC } from "react";

export const Navigation: FC = () => (
  <Group
    sx={{
      top: 0,
      height: 60,
      position: "sticky",
      backgroundColor: "transparent",
      backdropFilter: "blur(24px) saturate(1.8)",
      // boxShadow: "inset 0 -1px 0 0 hsla(0,0%,100%,.1)",
      zIndex: 3,
    }}
  >
    <Group
      pl="lg"
      spacing={5}
      align="flex-end"
      sx={{
        flex: 1,
      }}
    >
      <Logo />
      <Text
        variant="gradient"
        sx={{
          fontWeight: 900,
          fontSize: 13,
          userSelect: "none",
        }}
      >
        DASHBOARD
      </Text>
    </Group>
  </Group>
);
