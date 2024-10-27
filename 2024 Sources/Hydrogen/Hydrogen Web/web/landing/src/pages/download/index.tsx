import {
  Anchor,
  Button,
  Card,
  Group,
  Menu,
  Stack,
  Text,
  Title,
} from "@mantine/core";
import { useMediaQuery } from "@mantine/hooks";
import { Prism } from "@mantine/prism";
import {
  IconBrandAndroid,
  IconBrandApple,
  IconBrandWindows,
  IconChevronDown,
  IconDownload,
  IconFunction,
  IconHelp,
  IconStars,
} from "@tabler/icons";
import { useReleases } from "hooks/useReleases";
import { NextPage } from "next";
import Link from "next/link";

const macOSinstallScript =
  '/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/VersatileTeam/hydrogen-macos-installer/main/install.sh)"';

const Page: NextPage = () => {
  const { releases, os } = useReleases();
  const isSmallMobile = useMediaQuery("(max-width: 596px)");
  const isMobile = useMediaQuery("(max-width: 886px)");

  return (
    <Stack
      spacing={30}
      sx={{
        flexGrow: 1,
      }}
    >
      <Stack
        px={10}
        spacing="xs"
        align="center"
        justify="center"
        sx={{
          textAlign: "center",
          minHeight: "45vh",
          flexGrow: 1,
        }}
      >
        <Title
          mt={isSmallMobile ? 20 : undefined}
          size={55}
          variant="gradient"
          weight={700}
          sx={{
            lineHeight: isSmallMobile ? 1 : undefined,
          }}
        >
          Ready to get started?
        </Title>
        <Text
          color="dimmed"
          sx={{
            maxInlineSize: 700,
          }}
        >
          It&apos;s easy!{" "}
          {os === "MACOS"
            ? "Just copy the script below and paste it in your terminal."
            : "Just click the button below and you'll be set!"}{" "}
          {isSmallMobile ? null : (
            <>
              Don&apos;t hesitate to consult our{" "}
              <Anchor color="blue.2" href="/community" target="_blank">
                support
              </Anchor>{" "}
              through our community server if you think you need help.
            </>
          )}
        </Text>

        {os === "MACOS" ? (
          <Prism
            language="bash"
            py="md"
            sx={{
              maxInlineSize: isSmallMobile ? 200 : 600,
            }}
          >
            {macOSinstallScript}
          </Prism>
        ) : (
          <Button
            mt={isSmallMobile ? "lg" : "lg"}
            size={isSmallMobile ? "md" : "xl"}
            leftIcon={<IconDownload />}
            variant="gradient"
            gradient={
              {
                deg: 90,
                to: "#98bad9",
              } as any
            }
            onClick={() => {
              window.open("https://hydrogen.sh/download/android", "_blank");
            }}
          >
            Download Hydrogen for Android
          </Button>
        )}

        <Menu width={300} shadow="md">
          <Menu.Target>
            <Button
              size="xs"
              color="dark"
              variant="subtle"
              rightIcon={<IconChevronDown size={16} />}
            >
              Looking for other downloads?
            </Button>
          </Menu.Target>

          <Menu.Dropdown p={7}>
            {releases &&
              releases.map((release, i) => (
                <Menu.Item
                  key={i}
                  icon={
                    {
                      WINDOWS: <IconBrandWindows size={20} />,
                      ANDROID: <IconBrandAndroid size={20} />,
                      MACOS: <IconBrandApple size={20} />,
                      IOS: <IconBrandApple size={20} />,
                    }[release.platform!]
                  }
                  rightSection={<IconDownload size={20} />}
                  onClick={() => {
                    window.open(
                      `https://${
                        process.env.NEXT_PUBLIC_HOSTNAME
                      }/download/${release.platform.toLowerCase()}`,
                      "_blank"
                    );
                  }}
                >
                  {
                    {
                      WINDOWS: "Windows",
                      ANDROID: "Android",
                      MACOS: "macOS",
                      IOS: "iOS",
                    }[release.platform!]
                  }
                  <Text color="dimmed">Release {release.version} (Latest)</Text>
                </Menu.Item>
              ))}
            {/* <Menu.Item
              icon={<IconBrandWindows size={20} />}
              rightSection={<IconDownload size={20} />}
            >
              Windows
              <Text color="dimmed">Release 3.0.2 (Latest)</Text>
            </Menu.Item>
            <Menu.Item
              icon={<IconBrandApple size={20} />}
              rightSection={<IconDownload size={20} />}
            >
              macOS
              <Text color="dimmed">Release 1.4.2 (Latest)</Text>
            </Menu.Item>
            <Menu.Item
              icon={<IconBrandApple size={20} />}
              rightSection={<IconDownload size={20} />}
            >
              iOS
              <Text color="dimmed">Release 3.0.2 (Latest)</Text>
            </Menu.Item>
            <Menu.Item
              icon={<IconBrandAndroid size={20} />}
              rightSection={<IconDownload size={20} />}
            >
              Android
              <Text color="dimmed">Release 1.9.0 (Latest)</Text>
            </Menu.Item> */}
          </Menu.Dropdown>
        </Menu>
        <Text
          color="dimmed"
          size="xs"
          sx={{
            maxInlineSize: 350,
          }}
        >
          By downloading any of the software on this page, you acknowledge that
          you have read and understood, and agree to Hydrogen&apos;s{" "}
          <Anchor href="/community" target="_blank" color="dimmed">
            Terms of Service
          </Anchor>{" "}
          and{" "}
          <Anchor href="/community" target="_blank" color="dimmed">
            Privacy Policy
          </Anchor>
          .
        </Text>
      </Stack>

      <Group
        px="xl"
        py={50}
        position="apart"
        noWrap={!isMobile}
        sx={{
          borderTop: "1px solid #27272a",
          backgroundColor: "#101010",
          display: isSmallMobile ? "none" : "flex",
        }}
      >
        <Stack>
          <Title size={45} variant="gradient">
            What are you waiting for?
          </Title>
          <Text color="dimmed">
            Take advantage of Hydrogen&apos;s premium script execution platform
            right away. We&apos;ll be with you every step of the way.
          </Text>
          <Group spacing="xs">
            <Link passHref href="/download">
              <Button
                variant="gradient"
                gradient={
                  {
                    deg: 50,
                    to: "#76a4cf",
                  } as any
                }
              >
                Download Hydrogen
              </Button>
            </Link>
            {/* <Link passHref href="/community"> */}
            <Button
              variant="subtle"
              component="a"
              href="/community"
              target="_blank"
            >
              Join the Community
            </Button>
            {/* </Link> */}
          </Group>
        </Stack>
        <Group noWrap={!isMobile} align="stretch">
          <Card
            shadow="sm"
            p="lg"
            radius="md"
            withBorder
            sx={{
              display: "flex",
              flexDirection: "column",
              justifyContent: "space-between",
            }}
          >
            <Stack spacing={10}>
              <IconHelp size={36} />
              <Text weight={500}>Join the Discord community</Text>
              <Text
                size="sm"
                color="dimmed"
                sx={{
                  wordBreak: "break-word",
                  maxInlineSize: 1000,
                }}
              >
                We have worked hard to cultivate a community that is warm and
                friendly, with the goal of providing anyone and everyone with
                high-quality support. Regardless of the nature of the issue or
                the hour, we will find a way to assist you.
              </Text>
            </Stack>

            <Button
              component="a"
              href="/community"
              target="_blank"
              variant="light"
              color="blue"
              fullWidth
              mt="md"
              radius="md"
            >
              Join Server
            </Button>
          </Card>
          <Card
            shadow="sm"
            p="lg"
            radius="md"
            withBorder
            sx={{
              display: "flex",
              flexDirection: "column",
              justifyContent: "space-between",
            }}
          >
            <Stack spacing={10}>
              <IconFunction size={36} />
              <Text weight={500}>
                Explore Custom Functions - Available Soon
              </Text>
              <Text
                size="sm"
                color="dimmed"
                sx={{
                  wordBreak: "break-word",
                  maxInlineSize: 1000,
                }}
              >
                Our program comes with built-in support for a wide variety of
                helpful libraries and packages, which will make both the process
                of development and of playing a lot easier and more enjoyable.
              </Text>
            </Stack>

            <Button
              disabled
              variant="light"
              color="blue"
              fullWidth
              mt="md"
              radius="md"
            >
              View Documentation
            </Button>
          </Card>
          <Card
            shadow="sm"
            p="lg"
            radius="md"
            withBorder
            sx={{
              display: "flex",
              flexDirection: "column",
              justifyContent: "space-between",
            }}
          >
            <Stack spacing={10}>
              <IconStars size={36} />
              <Text weight={500}>Purchase Premium Plan - Available Soon</Text>
              <Text
                size="sm"
                color="dimmed"
                sx={{
                  wordBreak: "break-word",
                  maxInlineSize: 1000,
                }}
              >
                Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do
                eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut
                enim ad minim veniam, quis nostrud exercitation ullamco laboris
                nisi ut aliquip ex ea commodo consequat.
              </Text>
            </Stack>

            <Button
              disabled
              variant="light"
              color="blue"
              fullWidth
              mt="md"
              radius="md"
            >
              View Premium Plans
            </Button>
          </Card>
        </Group>
      </Group>
    </Stack>
  );
};

export default Page;
