import {
  Accordion,
  Button,
  Container,
  Group,
  Image,
  SimpleGrid,
  Stack,
  Text,
  Title,
} from "@mantine/core";
import { useMediaQuery } from "@mantine/hooks";
import { NextLink } from "@mantine/next";
import {
  IconApi,
  IconAppWindow,
  IconArrowRight,
  IconBrandAndroid,
  IconBrandApple,
  IconBrandWindows,
  IconClock,
  IconCode,
  IconHelp,
  IconLock,
} from "@tabler/icons";
import { FeatureCard } from "components/Cards/FeatureCard";
import { Countdown } from "components/Countdown";
import { useReleases } from "hooks/useReleases";
import { NextPage } from "next";
import Link from "next/link";

// const testimonials = [
//   {
//     rating: 4,
//     username: "Reha",
//     review:
//       "Our software comes with built-in support for many useful libraries and packages that will make development as well as playtime easy and fun!",
//   },
//   {
//     rating: 4,
//     username: "a",
//     review:
//       "Our software comes with built-in support for many useful libraries and packages that will make development as well as playtime easy and fun!",
//   },
// ];

const features = [
  {
    icon: IconApi,
    title: "Powerful Execution Capabilities",
    description:
      "Our software comes with built-in support for many useful libraries and packages that will make development as well as playtime easy and fun!",
  },
  {
    icon: IconHelp,
    title: "24/7 Support Team",
    description:
      "Our welcoming community has been built up to provide anyone and everyone with quality support, no matter the problem or time, we'll get you the help you need.",
  },
  {
    icon: IconLock,
    title: "Cross-Platform",
    description:
      "We've designed our products to be compatible with almost every platform of every game we come across, game on the go with Android and come back home to Windows.",
  },
  {
    icon: IconCode,
    title: "Expansive Script Library",
    description:
      "A vast public script library is available for use with every Hydrogen product, allowing you to execute scripts easily and without hassle.",
  },
  {
    icon: IconClock,
    title: "Speedy Updates",
    description:
      "Our extremely quick developers are dedicated to providing everyone with the most pleasurable experience possible, that includes quick and easy to download updates.",
  },
  {
    icon: IconAppWindow,
    title: "And of course, a Beautiful UI",
    description:
      "Hand crafted user-interfaces designed carefully for each platform, made with perfection and maximum productivity in mind.",
  },
];

const Page: NextPage = () => {
  const isMobile = useMediaQuery("(max-width: 800px)");
  const { formattedOs, osIcon, downloadText } = useReleases();

  return (
    <Stack
      spacing={100}
      pt={65}
      pb={100}
      sx={{
        flexGrow: 1,
      }}
    >
      <Stack align="center" spacing="xl" px="sm">
        <Stack
          spacing="xl"
          align="center"
          sx={{
            textAlign: "center",
          }}
        >
          <Title
            variant="gradient"
            size={70}
            weight={700}
            sx={{
              minHeight: 150,
              maxWidth: 800,
              lineHeight: 1,
              wordBreak: "break-word",
              "@media (max-width: 409px)": {
                fontSize: 60,
              },
            }}
          >
            Enhancing the scripting experience
          </Title>

          <Text
            color="dimmed"
            sx={{
              fontSize: 20,
            }}
          >
            A huge step forward in script execution, now available on{" "}
            <Text component="span" weight={500}>
              Android & macOS
            </Text>
            .
          </Text>

          <Countdown
            endTime={new Date("10/1/2022").getTime()}
            extraContent={
              <Group spacing="xs">
                Left Until Release
                <IconArrowRight size={20} />
              </Group>
            }
            finishedContent={
              <Stack spacing={25} align="center">
                <Button
                  size="lg"
                  href="/download"
                  component={NextLink}
                  leftIcon={osIcon}
                  rightIcon={<IconArrowRight />}
                  variant="gradient"
                  gradient={
                    {
                      deg: 90,
                      to: "#98bad9",
                    } as any
                  }
                  sx={{
                    width: "min-content",
                  }}
                >
                  {downloadText}
                </Button>
                <Stack>
                  <Text color="dimmed" size={15}>
                    And will{" "}
                    <Text component="span" weight={600}>
                      soon
                    </Text>{" "}
                    be available on all of your favorite platforms.
                  </Text>
                  <Group position="center">
                    {["ios", "windows"]
                      // .filter((x) => x !== os)
                      .map((os, i) => (
                        <Group
                          key={i}
                          spacing={5}
                          sx={{
                            color: "#909296",
                          }}
                        >
                          {
                            {
                              undetermined: null,
                              macos: <IconBrandApple />,
                              ios: <IconBrandApple />,
                              windows: <IconBrandWindows />,
                              android: <IconBrandAndroid />,
                              linux: null,
                            }[os]
                          }
                          <Text
                            color="dimmed"
                            weight={500}
                            sx={{
                              userSelect: "none",
                            }}
                          >
                            {
                              {
                                undetermined: "",
                                macos: "macOS",
                                ios: "iOS",
                                windows: "Windows",
                                android: "Android",
                                linux: "",
                              }[os]
                            }
                          </Text>
                        </Group>
                      ))}
                  </Group>
                </Stack>
              </Stack>
            }
          />
        </Stack>
        {/* <Image
          mt={30}
          alt="ui"
          src="/UI-android - Copy.png"
          styles={{
            root: {
              maxWidth: 1000,
            },
            image: {
              "-webkit-filter": "drop-shadow(5px 5px 5px #222)",
              filter: "drop-shadow(5px 5px 5px #222)",
            },
          }}
        /> */}
        <Image
          mt={70}
          alt="ui"
          src="/ui.webp"
          styles={{
            root: {
              maxWidth: 800,
            },
            image: {
              border: "3px solid #8dc1f0",
              borderRadius: 6,
              boxShadow: "#c0e1ff 1px 1px 120px -35px",
            },
          }}
        />
      </Stack>

      <Stack
        px="xl"
        py={70}
        spacing={50}
        align="center"
        sx={{
          borderTop: "1px solid #27272a",
          borderBottom: "1px solid #27272a",
          backgroundColor: "#101010",
        }}
      >
        <Stack
          spacing="xs"
          align="center"
          sx={{
            textAlign: "center",
          }}
        >
          <Title
            order={2}
            sx={{
              fontWeight: 500,
            }}
          >
            Bringing more to the table than just a fancy UI
          </Title>
          <Text color="dimmed">
            Jam packed with more features than any other executor
          </Text>
        </Stack>

        <SimpleGrid
          cols={3}
          spacing={50}
          breakpoints={[
            { maxWidth: 980, cols: 2, spacing: "xl" },
            { maxWidth: 755, cols: 1, spacing: "xl" },
          ]}
          sx={{
            maxWidth: 1200,
          }}
        >
          {features.map(({ icon, title, description }, i) => (
            <FeatureCard
              key={i}
              icon={icon}
              title={title}
              description={description}
            />
          ))}
        </SimpleGrid>
      </Stack>

      <Stack px="xl" spacing={50} align="center">
        <Stack
          spacing="xs"
          align="center"
          sx={{
            textAlign: "center",
            maxWidth: 700,
          }}
        >
          <Title
            order={2}
            sx={{
              fontWeight: 500,
            }}
          >
            Frequently Asked Questions
          </Title>
          <Text color="dimmed">
            Here are some commonly asked questions, if you have a question that
            isn&apos;t listed here, don&apos;t be shy to visit our
            <Text
              component={NextLink}
              href="/help"
              sx={(theme) => ({
                ":hover": {
                  color: theme.colors.gray[5],
                },
              })}
            >
              {" "}
              support page{" "}
            </Text>
            or ask someone in our{" "}
            <Text
              underline
              component={NextLink}
              href="/community"
              sx={(theme) => ({
                ":hover": {
                  color: theme.colors.gray[5],
                },
              })}
            >
              community
            </Text>
            .
          </Text>
        </Stack>

        <Accordion
          multiple
          sx={{
            width: "100%",
            maxWidth: 1200,
          }}
        >
          <Accordion.Item value="cost">
            <Accordion.Control>How much does Hydrogen cost?</Accordion.Control>
            <Accordion.Panel>
              We provide a completely free version that requires users to go
              through a key system (Hydrogen Gateway), as well as a one-time
              lifetime payment of $30 that grants you ad-free access to all of
              our products.
            </Accordion.Panel>
          </Accordion.Item>
          <Accordion.Item value="purchase">
            <Accordion.Control>How do I purchase Hydrogen?</Accordion.Control>
            <Accordion.Panel>
              At the moment, Hydrogen AdLess is not available for purchase, we
              will be providing updates on when our ad-free version is available
              for purchase. Until then, you must go through the gateway.
            </Accordion.Panel>
          </Accordion.Item>
          <Accordion.Item value="guide">
            <Accordion.Control>
              What do I do after buying Hydrogen?
            </Accordion.Control>
            <Accordion.Panel>
              Nothing! Simply enter any game and you will automatically be
              granted access to all Hydrogen products without any
              advertisements.
            </Accordion.Panel>
          </Accordion.Item>
          <Accordion.Item value="showcase">
            <Accordion.Control>
              Is there anywhere I can see Hydrogen in action?
            </Accordion.Control>
            <Accordion.Panel>
              Of course! Hydrogen video demonstrations are available in our
              community Discord server.
            </Accordion.Panel>
          </Accordion.Item>
          <Accordion.Item value="difference">
            <Accordion.Control>
              What makes Hydrogen different from other exploits?
            </Accordion.Control>
            <Accordion.Panel>
              Our commitment to offer consumers with an easy-to-use and
              enjoyable experience has led us to construct a robust scripting
              ecosystem developed by industry leaders. You will, without a
              doubt, receive what you pay for.
            </Accordion.Panel>
          </Accordion.Item>
        </Accordion>
      </Stack>

      {/* 
      <Stack
        px="xl"
        py={70}
        spacing={50}
        align="center"
        sx={{
          borderTop: "1px solid #27272a",
          borderBottom: "1px solid #27272a",
          backgroundColor: "#101010",
        }}
      >
        <Stack
          spacing="xs"
          align="center"
          sx={{
            textAlign: "center",
          }}
        >
          <Title
            order={2}
            sx={{
              fontWeight: 500,
            }}
          >
            Don&apos;t just take our word for it...
          </Title>
          <Text color="dimmed">
            Over 5,000 happy customers can attest to our pure dedication and
            love for our community.
          </Text>
        </Stack>

        <Stack
          spacing="xl"
          sx={{
            maxWidth: "calc(100vw - 48px)",
          }}
        >
          <Marquee
            pauseOnHover
            direction="left"
            gradient={!isMobile}
            gradientColor={[16, 16, 16]}
            speed={isMobile ? 50 : 20}
          >
            {testimonials.map(({ rating, username, review }, i) => (
              <Box px="xl" key={i}>
                <ReviewCard
                  rating={rating}
                  username={username}
                  review={review}
                />
              </Box>
            ))}
          </Marquee>
          <Marquee
            pauseOnHover
            direction="right"
            gradient={!isMobile}
            gradientColor={[16, 16, 16]}
            speed={isMobile ? 50 : 20}
          >
            {testimonials.map(({ rating, username, review }, i) => (
              <Box px="xl" key={i}>
                <ReviewCard
                  rating={rating}
                  username={username}
                  review={review}
                />
              </Box>
            ))}
          </Marquee>
        </Stack>
      </Stack> */}

      <Container
        size="xl"
        sx={{
          width: "100%",
          borderRadius: 10,
          position: "relative",
          backgroundColor: "#101010",
          marginTop: 50,
          "@media (max-width: 1325px)": {
            borderRadius: 0,
            marginTop: "initial",
          },
        }}
      >
        <Group py={60} px={40} spacing={60}>
          <Stack
            sx={{
              maxWidth: 600,
            }}
          >
            <Title
              size={45}
              variant="gradient"
              sx={{
                lineHeight: 1.2,
              }}
            >
              What&apos;s with the holdup?
            </Title>
            <Text>It&apos;s time to ditch the competition</Text>
            <Text color="dimmed">
              Take advantage of Hydrogen&apos;s premium script execution
              platform right away. We&apos;ll be with you every step of the way.
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

          <Image
            alt="ui"
            src="/ui.webp"
            radius={6}
            styles={{
              root: {
                cursor: "pointer",
                "@media (min-width: 1325px)": {
                  display: "none",
                },
              },
              image: {
                boxShadow: "#fff 0.5px 0.5px 50px -60px",
                borderBottomLeftRadius: 0,
                transitionDuration: "300ms",
                ":hover": {
                  scale: "1.01",
                },
              },
            }}
          />
        </Group>

        <Image
          alt="ui"
          src="/ui.webp"
          fit="contain"
          radius={6}
          height="auto"
          width={650}
          styles={{
            root: {
              cursor: "pointer",
              position: "absolute",
              bottom: 0,
              right: 0,
              border: "1px solid #27272a",
              borderBottom: "none",
              borderRadius: 6,
              "@media (max-width: 1325px)": {
                display: "none",
              },
            },
            image: {
              boxShadow: "#fff 0.5px 0.5px 50px -60px",
              borderBottomLeftRadius: 0,
              transitionDuration: "300ms",
              ":hover": {
                scale: "1.01",
              },
            },
          }}
        />
      </Container>
    </Stack>
  );
};

export default Page;
