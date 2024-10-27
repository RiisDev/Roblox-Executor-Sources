import { Button, Group, MediaQuery, Text } from "@mantine/core";
import { useMediaQuery } from "@mantine/hooks";
import { NextLink } from "@mantine/next";
import { Logo } from "components/Logo";
import Link from "next/link";
import { FC } from "react";

export const Navigation: FC = () => {
  const isMobile = useMediaQuery("(max-width: 800px)");

  return (
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
        sx={{
          flex: 1,
        }}
      >
        <Logo />
      </Group>
      <MediaQuery
        smallerThan="sm"
        styles={{
          display: "none",
        }}
      >
        <Group
          noWrap
          spacing={isMobile ? 30 : 50}
          position="center"
          sx={{
            flex: 1,
          }}
        >
          <Text
            component={NextLink}
            href="/download"
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
            href="/docs"
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
            component="a"
            target="_blank"
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
            Community
          </Text>
          <Text
            component="a"
            target="_blank"
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
            Showcase
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
      <Group
        spacing="xs"
        pr="lg"
        position="right"
        sx={{
          flex: 1,
          flexWrap: "nowrap",
        }}
      >
        <Link passHref href="/download">
          <Button
            size="xs"
            variant="gradient"
            gradient={
              {
                deg: 20,
                to: "#76a4cf",
              } as any
            }
          >
            Download
          </Button>
        </Link>
      </Group>
      {/* <AuthDialog
        pr="lg"
        position="right"
        sx={{
          flex: 1,
          flexWrap: "nowrap",
        }}
      /> */}
    </Group>
  );
};
