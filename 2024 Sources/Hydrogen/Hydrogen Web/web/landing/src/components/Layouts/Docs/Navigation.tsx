import {
  Group,
  Kbd,
  MediaQuery,
  Stack,
  Tabs,
  Text,
  TextInput,
  useMantineTheme,
} from "@mantine/core";
import { useFocusWithin, useHotkeys } from "@mantine/hooks";
import { NextLink } from "@mantine/next";
import { IconArrowRight, IconSearch } from "@tabler/icons";
import { AuthDialog } from "components/Dialogs/AuthDialog";
import { Logo } from "components/Logo";
import { FC } from "react";

export const Navigation: FC = () => {
  const theme = useMantineTheme();
  const { ref, focused } = useFocusWithin();

  useHotkeys([["/", () => ref.current.focus()]]);

  return (
    <Stack spacing={0}>
      <Group
        px="lg"
        align="center"
        position="apart"
        sx={{
          top: 0,
          height: 60,
          position: "sticky",
          backgroundColor: "transparent",
          backdropFilter: "blur(24px) saturate(1.8)",
          // boxShadow: "inset 0 -1px 0 0 hsla(0,0%,100%,.1)",
          zIndex: 1,
        }}
      >
        <Group spacing={5} align="flex-end">
          <Logo />
          <Text
            variant="gradient"
            sx={{
              fontWeight: 700,
              fontSize: 13,
              userSelect: "none",
            }}
          >
            DOCS
          </Text>
        </Group>

        <MediaQuery
          smallerThan="sm"
          styles={{
            display: "none",
          }}
        >
          <TextInput
            ref={ref}
            icon={<IconSearch size={14} />}
            placeholder="Search documentation"
            rightSection={
              <Kbd
                mr="sm"
                sx={{
                  fontSize: 11,
                  lineHeight: 1,
                  userSelect: "none",
                  opacity: focused ? 0 : "initial",
                  visibility: focused ? "hidden" : "initial",
                  transition: "visibility 0.1s, opacity 0.1s linear",
                }}
              >
                /
              </Kbd>
            }
            sx={{
              minWidth: 300,
            }}
          />
        </MediaQuery>

        <AuthDialog />
      </Group>

      <Tabs
        defaultValue="home"
        styles={{
          tabsList: {
            paddingLeft: 10,
            paddingRight: 30,
          },
          tab: {
            borderRadius: 1,
            fontWeight: 500,
            paddingLeft: 13,
            paddingRight: 13,
            "&[data-active]": {
              fontWeight: 600,
              color: theme.colors.blue[3],
              borderColor: theme.colors.blue[3],
            },
            "&[data-active]:hover": {
              backgroundColor: "transparent",
              borderColor: theme.colors.blue[2],
            },
            ":hover": {
              backgroundColor: "transparent",
              borderColor: theme.colors.dark[3],
            },
          },
        }}
      >
        <Tabs.List
          sx={{
            justifyContent: "space-between",
          }}
        >
          <Group spacing="sm">
            <Tabs.Tab value="home">Home</Tabs.Tab>
            <Tabs.Tab value="secure-auth">Secure Auth</Tabs.Tab>
            <Tabs.Tab value="cloud-configs">Cloud Configs</Tabs.Tab>
            <Tabs.Tab value="ui-designer">UI Designer</Tabs.Tab>
            <Tabs.Tab value="api-reference">API Reference</Tabs.Tab>
          </Group>
          <Group>
            <Text
              component={NextLink}
              href="/support"
              sx={(theme) => ({
                fontSize: 12,
                fontWeight: 600,
                color: theme.colors.dark[2],
                ":hover": {
                  color: theme.colors.dark[1],
                },
              })}
            >
              Support
            </Text>
            <Text
              component={NextLink}
              href="/dashboard"
              sx={(theme) => ({
                fontSize: 12,
                fontWeight: 600,
                color: theme.colors.dark[2],
                ":hover": {
                  color: theme.colors.dark[1],
                },
              })}
            >
              <Group align="center" spacing={1}>
                Dashboard
                <IconArrowRight
                  size={16}
                  style={{
                    paddingBottom: 1,
                  }}
                />
              </Group>
            </Text>
          </Group>
        </Tabs.List>

        <Tabs.Panel value="home" pt="xs">
          {" "}
        </Tabs.Panel>

        <Tabs.Panel value="secure" pt="xs">
          {" "}
        </Tabs.Panel>

        <Tabs.Panel value="api" pt="xs">
          {" "}
        </Tabs.Panel>
      </Tabs>
    </Stack>
  );
};
