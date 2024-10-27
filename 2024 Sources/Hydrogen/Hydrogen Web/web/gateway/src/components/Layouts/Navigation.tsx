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
        GATEWAY
      </Text>
    </Group>
    <MediaQuery
      smallerThan="sm"
      styles={{
        display: "none",
      }}
    >
      <Group noWrap pr="lg" spacing={25} position="center">
        <Text
          component={NextLink}
          href={`https://${process.env.NEXT_PUBLIC_HOSTNAME}/download`}
          sx={(theme) => ({
            fontSize: 14,
            fontWeight: 600,
            color: theme.colors.dark[2],
            ":hover": {
              color: theme.colors.dark[1],
            },
          })}
        >
          Download
        </Text>
        {/* <Text
          component={NextLink}
          href={`https://${process.env.NEXT_PUBLIC_HOSTNAME}/docs`}
          sx={(theme) => ({
            fontSize: 14,
            fontWeight: 600,
            color: theme.colors.dark[2],
            ":hover": {
              color: theme.colors.dark[1],
            },
          })}
        >
          Developers
        </Text> */}
        <Text
          component={NextLink}
          href={`https://${process.env.NEXT_PUBLIC_HOSTNAME}/community`}
          sx={(theme) => ({
            fontSize: 14,
            fontWeight: 600,
            color: theme.colors.dark[2],
            ":hover": {
              color: theme.colors.dark[1],
            },
          })}
        >
          Community
        </Text>
        {/* <Text
          component={NextLink}
          href="/community"
          sx={(theme) => ({
            fontSize: 14,
            fontWeight: 600,
            color: theme.colors.dark[2],
            ":hover": {
              color: theme.colors.dark[1],
            },
          })}
        >
          Pricing
        </Text> */}
      </Group>
    </MediaQuery>
  </Group>
);
